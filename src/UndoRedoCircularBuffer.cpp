#include <UndoRedoCircularBuffer.hpp>
#include <Log.hpp>
#include <string>
#include <stack> // to reverse queue

const UndoRedoCircularBuffer::URCB_T UndoRedoCircularBuffer::PUSH_FRONT = 1;
const UndoRedoCircularBuffer::URCB_T UndoRedoCircularBuffer::PUSH_FRONT_WRAPPED = 2;
const UndoRedoCircularBuffer::URCB_T UndoRedoCircularBuffer::PUSH_BACK = 3;
const UndoRedoCircularBuffer::URCB_T UndoRedoCircularBuffer::PUSH_BACK_WRAPPED = 4;

const UndoRedoCircularBuffer::URCB_T UndoRedoCircularBuffer::POP_FRONT = 5;
const UndoRedoCircularBuffer::URCB_T UndoRedoCircularBuffer::POP_BACK = 6;

std::string cmdToString(int cmd) {
    switch (cmd) {
        case UndoRedoCircularBuffer::PUSH_FRONT: return "PUSH FRONT";
        case UndoRedoCircularBuffer::PUSH_FRONT_WRAPPED: return "PUSH FRONT WRAPPED";
        case UndoRedoCircularBuffer::PUSH_BACK: return "PUSH BACK";
        case UndoRedoCircularBuffer::PUSH_BACK_WRAPPED: return "PUSH BACK WRAPPED";

        case UndoRedoCircularBuffer::POP_FRONT: return "POP FRONT";
        case UndoRedoCircularBuffer::POP_BACK: return "POP BACK";
        default: return "UNKNOWN COMMAND";
    }
}

UndoRedoCircularBuffer::UndoRedoCircularBuffer(size_t size) : UndoRedoCircularBuffer(size, size, size) {}

UndoRedoCircularBuffer::UndoRedoCircularBuffer(size_t size, size_t undo_redo_size) : UndoRedoCircularBuffer(size, undo_redo_size, undo_redo_size) {}

UndoRedoCircularBuffer::UndoRedoCircularBuffer(size_t size, size_t undo_size, size_t redo_size) :
    main(new rigtorp::SPSCQueue<URCB_T>(size)),
    undo_(new rigtorp::SPSCQueue<URCB_T>(undo_size*2)),
    redo_(new rigtorp::SPSCQueue<URCB_T>(redo_size*2)),
    DEBUG_COMMANDS(true),
    DEBUG_ACTIONS(false),
    DEBUG_STATE(false)
{}

UndoRedoCircularBuffer::~UndoRedoCircularBuffer() {
    delete main;
    delete undo_;
    delete redo_;
}

size_t UndoRedoCircularBuffer::size() const {
    return main->size();
}

bool UndoRedoCircularBuffer::empty() const {
    return main->empty();
}

UndoRedoCircularBuffer::URCB_T UndoRedoCircularBuffer::front() const {
    return front(main);
}

UndoRedoCircularBuffer::URCB_T UndoRedoCircularBuffer::back() const {
    return back(main);
}

void UndoRedoCircularBuffer::push_front(rigtorp::SPSCQueue<URCB_T> * buf, const URCB_T & value) {
    // IMPORTANT: NOT THREAD SAFE -
    //                  it could be popped leading to a block until data is pushed to it
    //                      T1 pop(); // size 0
    //                      T2 pop(); // block cus size 0; push(value);
    //                  or it could be pushed leading to a block until data is popped from it
    //                      T2 pop();
    //                      T1 push(); // size 3, capacity 3
    //                      T2 push(value); // block cus size == capacity
    if (buf->size() == buf->capacity()) buf->pop();
    buf->push(value);
}

void reverse(rigtorp::SPSCQueue<UndoRedoCircularBuffer::URCB_T> * buf) {
    std::deque<UndoRedoCircularBuffer::URCB_T> Deque;
    while (!buf->empty()) {
        Deque.push_front(*buf->front());
        buf->pop();
    }
    while (!Deque.empty()) {
        buf->push(Deque.front());
        Deque.pop_front();
    }
}

void UndoRedoCircularBuffer::push_back(rigtorp::SPSCQueue<URCB_T> * buf, const URCB_T & value) {
    // reverse the queue    1,2,3 > reverse > 3,2,1
    reverse(buf);
    // push to it           3,2,1 > push 5 > 2,1,5
    push_front(buf, value);
    // reverse it again     2,1,5 > reverse > 5,1,2
    reverse(buf);
}

UndoRedoCircularBuffer::URCB_T UndoRedoCircularBuffer::pop_front__(rigtorp::SPSCQueue<URCB_T> * buf) {
    if (buf->empty()) return 0;
    URCB_T value = *buf->front();
    buf->pop();
    return value;
}

UndoRedoCircularBuffer::URCB_T UndoRedoCircularBuffer::pop_back__(rigtorp::SPSCQueue<URCB_T> * buf) {
    if (buf->empty()) return 0;
    size_t size = buf->size();

    if (size == 1) return pop_front__(buf);

    rigtorp::SPSCQueue<URCB_T> tmp(size);
    URCB_T back = 0;
    for (size_t i = 0; i < size; i++) {
        URCB_T * value = buf->front();
        if (i == size-1) back = *value;
        tmp.push(*value);
        buf->pop();
    }
    for (size_t i = 0; i < size-1; i++) {
        buf->push(*tmp.front());
        tmp.pop();
    }
    return back;
}

UndoRedoCircularBuffer::URCB_T UndoRedoCircularBuffer::front(rigtorp::SPSCQueue<URCB_T> * buf) {
    if (buf->empty()) return 0;
    return *buf->front();
}

UndoRedoCircularBuffer::URCB_T UndoRedoCircularBuffer::back(rigtorp::SPSCQueue<URCB_T> * buf) {
    if (buf->empty()) return 0;
    size_t size = buf->size();
    rigtorp::SPSCQueue<URCB_T> tmp(size);
    URCB_T back = 0;
    for (size_t i = 0; i < size; i++) {
        URCB_T value = *buf->front();
        if (i == size-1) back = value;
        tmp.push(value);
        buf->pop();
    }
    for (size_t i = 0; i < size; i++) {
        buf->push(*tmp.front());
        tmp.pop();
    }
    return back;
}

UndoRedoCircularBuffer::Command::Command(URCB_T c, URCB_T d) {
    cmd = c;
    data = d;
}

UndoRedoCircularBuffer::Command UndoRedoCircularBuffer::push_front(rigtorp::SPSCQueue<URCB_T> * buf, URCB_T cmd, URCB_T data) {
    push_front(buf, cmd);
    push_front(buf, data);
    return {cmd, data};
}

UndoRedoCircularBuffer::Command UndoRedoCircularBuffer::pop_back_(rigtorp::SPSCQueue<URCB_T> * buf) {
    URCB_T data = pop_back__(buf);
    URCB_T cmd = pop_back__(buf);
    return {cmd, data};
}

void UndoRedoCircularBuffer::push_front(URCB_T n) const {
    if (DEBUG_COMMANDS) LOG_MAGNUM_DEBUG << "push front " << n << std::endl;
    if (main->size() == main->capacity()) {
        push_front(undo_, PUSH_FRONT_WRAPPED, front());
    } else {
        push_front(undo_, PUSH_FRONT, n);
    }
    push_front(main, n);
}

void UndoRedoCircularBuffer::push_back(URCB_T n) const {
    if (DEBUG_COMMANDS) LOG_MAGNUM_DEBUG << "push back " << n << std::endl;
    if (main->size() == main->capacity()) {
        push_front(undo_, PUSH_BACK_WRAPPED, front());
    } else {
        push_front(undo_, PUSH_BACK, n);
    }
    push_back(main, n);
}

UndoRedoCircularBuffer::URCB_T UndoRedoCircularBuffer::pop_front() const {
    if (DEBUG_COMMANDS) LOG_MAGNUM_DEBUG << "pop front" << std::endl;
    if (main->empty()) return 0;
    URCB_T r = pop_front__(main);
    push_front(undo_, POP_FRONT);
    push_front(undo_, r);
    return r;
}

UndoRedoCircularBuffer::URCB_T UndoRedoCircularBuffer::pop_back() const {
    if (DEBUG_COMMANDS) LOG_MAGNUM_DEBUG << "pop back" << std::endl;
    if (main->empty()) return 0;
    URCB_T r = pop_back__(main);
    push_front(undo_, POP_BACK);
    push_front(undo_, r);
    return r;
}

void UndoRedoCircularBuffer::undo() const {
    if (undo_->empty()) return;
    Command command = pop_back_(undo_);
    std::string cmd = cmdToString(command.cmd);
    switch (command.cmd) {
        case PUSH_FRONT: {
            LOG_MAGNUM_DEBUG << "undo " << cmd << std::endl;
            push_front(redo_, command.cmd, pop_back__(main));
            break;
        }
        case PUSH_FRONT_WRAPPED: {
            LOG_MAGNUM_DEBUG << "undo " << cmd << std::endl;
            URCB_T value = back(main);
            push_back(main, command.data);
            push_front(redo_, command.cmd, value);
            break;
        }
        case POP_FRONT: {
            LOG_MAGNUM_DEBUG << "undo " << cmd << std::endl;
            push_back(main, command.data);
            push_front(redo_, command.cmd, command.data);
            break;
        }
        default: {
            LOG_MAGNUM_FATAL << "undo " << cmd << std::endl;
        }
    }
}

void UndoRedoCircularBuffer::redo() const {
    if (redo_->empty()) return;
    Command command = pop_back_(redo_);
    std::string cmd = cmdToString(command.cmd);
    switch (command.cmd) {
        case PUSH_FRONT: {
            LOG_MAGNUM_DEBUG << "redo " << cmd << std::endl;
            push_front(undo_, command.cmd, command.data);
            push_front(main, command.data);
            break;
        }
        case PUSH_FRONT_WRAPPED: {
            LOG_MAGNUM_DEBUG << "redo " << cmd << std::endl;
            push_front(undo_, command.cmd, front(main));
            push_front(main, command.data);
            break;
        }
        case POP_FRONT: {
            LOG_MAGNUM_DEBUG << "redo " << cmd << std::endl;
            push_front(undo_, command.cmd, command.data);
            main->pop();
            break;
        }
        default: {
            LOG_MAGNUM_FATAL << "redo " << cmd << std::endl;
        }
    }
}

template<typename... Args>
std::string UndoRedoCircularBuffer::format(const std::string &format, Args... args) const {
    int size = snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
    if( size <= 0 ) {
        throw std::runtime_error("Error during formatting.");
    }
    std::unique_ptr<char[]> buf( new char[ size ] );
    snprintf( buf.get(), size, format.c_str(), args ... );
    return std::string( buf.get(), buf.get() + size - 1 );
}

std::string UndoRedoCircularBuffer::toString(rigtorp::SPSCQueue<URCB_T> * buf) const {
    size_t size = buf->capacity();
    rigtorp::SPSCQueue<URCB_T> tmp(size*2);
    std::string string = format("(size = %02zu, capacity = %02zu)", buf->size(), size);
    if (buf == main) {
        for (size_t i = 0; i < size; i++) {
            URCB_T *ptr = buf->front();
            URCB_T val = 0;
            tmp.push(ptr != nullptr);
            if (ptr != nullptr) {
                val = *ptr;
                buf->pop();
                string += format("%23ld", val);
            } else {
                string += format("%23c", '_');
            }
            if (i+1 != size) string += ",";
            tmp.push(val);
        }
    } else {
        for (size_t i = 0; i < size; i += 2) {
            URCB_T *command = buf->front();
            URCB_T command_cmd = 0;
            URCB_T command_value = 0;
            tmp.push(command != nullptr);
            if (command != nullptr) {
                command_cmd = *command;
                buf->pop();
                command_value = *buf->front();
                buf->pop();
                std::string cmd = cmdToString(command_cmd);
                string += format("%4s{ %11s,%3ld }", " ", cmd.data(), command_value);
            } else {
                string += format("%22s_", " ");
            }
            tmp.push(command_cmd);
            tmp.push(command != nullptr);
            tmp.push(command_value);
            if (i+2 != size) string += ",";
        }
    }
    for (size_t i = 0; i < size; i++) {
        URCB_T shouldPush = *tmp.front();
        tmp.pop();
        URCB_T * ptr = tmp.front();
        URCB_T val = 0;
        if (ptr != nullptr) {
            val = *ptr;
            tmp.pop();
            if (shouldPush) buf->push(val);
        }
    }
    return string;
}

std::string UndoRedoCircularBuffer::toString() const {
    return "MAIN: " + toString(main) +
           "\nUNDO: " + toString(undo_) +
           "\nREDO: " + toString(redo_);
}
