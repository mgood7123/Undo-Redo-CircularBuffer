#include <UndoRedoCircularBuffer.hpp>
#include <Log.hpp>
#include <string>
#include <stack> // to reverse queue

const int UndoRedoCircularBuffer::ADD = 1;
const int UndoRedoCircularBuffer::ADD_WRAPPED = 2;
const int UndoRedoCircularBuffer::REMOVE = 3;

std::string cmdToString(int cmd) {
    switch (cmd) {
        case UndoRedoCircularBuffer::ADD: return "ADD";
        case UndoRedoCircularBuffer::ADD_WRAPPED: return "ADD_WRAPPED";
        case UndoRedoCircularBuffer::REMOVE: return "REMOVE";
        default: return "UNKNOWN";
    }
}

UndoRedoCircularBuffer::UndoRedoCircularBuffer(int size) : UndoRedoCircularBuffer(size, size, size) {}

UndoRedoCircularBuffer::UndoRedoCircularBuffer(int size, int undo_redo_size) : UndoRedoCircularBuffer(size, undo_redo_size, undo_redo_size) {}

UndoRedoCircularBuffer::UndoRedoCircularBuffer(int size, int undo_size, int redo_size) :
    main(new rigtorp::SPSCQueue<int>(size)),
    undo_(new rigtorp::SPSCQueue<int>(undo_size*2)),
    redo_(new rigtorp::SPSCQueue<int>(redo_size*2)),
    DEBUG_COMMANDS(true),
    DEBUG_ACTIONS(false),
    DEBUG_STATE(false)
{}

UndoRedoCircularBuffer::~UndoRedoCircularBuffer() {
    delete main;
    delete undo_;
    delete redo_;
}

int UndoRedoCircularBuffer::size() const {
    return main->size();
}

int UndoRedoCircularBuffer::front() const {
    return front(main);
}

int UndoRedoCircularBuffer::back() const {
    return back(main);
}

int UndoRedoCircularBuffer::pop_front() const {
    return pop_front(main);
}

int UndoRedoCircularBuffer::pop_back() const {
    return pop_back(main);
}

void UndoRedoCircularBuffer::push_front(rigtorp::SPSCQueue<int> * buf, const int & value) {
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

void reverse(rigtorp::SPSCQueue<int> * buf) {
    std::deque<int> Deque;
    while (!buf->empty()) {
        Deque.push_front(*buf->front());
        buf->pop();
    }
    while (!Deque.empty()) {
        buf->push(Deque.front());
        Deque.pop_front();
    }
}

void UndoRedoCircularBuffer::push_back(rigtorp::SPSCQueue<int> * buf, const int & value) {
    // reverse the queue    1,2,3 > reverse > 3,2,1
    reverse(buf);
    // push to it           3,2,1 > push 5 > 2,1,5
    push_front(buf, value);
    // reverse it again     2,1,5 > reverse > 5,1,2
    reverse(buf);
}

int UndoRedoCircularBuffer::pop_front(rigtorp::SPSCQueue<int> * buf) {
    int * value = buf->front();
    buf->pop();
    return *value;
}

int UndoRedoCircularBuffer::pop_back(rigtorp::SPSCQueue<int> * buf) {
    size_t size = buf->size();

    if (size == 1) return pop_front(buf);

    rigtorp::SPSCQueue<int> tmp(size);
    int back = 0;
    for (int i = 0; i < size; i++) {
        int * value = buf->front();
        if (i == size-1) back = *value;
        tmp.push(*value);
        buf->pop();
    }
    for (int i = 0; i < size-1; i++) {
        buf->push(*tmp.front());
        tmp.pop();
    }
    return back;
}

int UndoRedoCircularBuffer::front(rigtorp::SPSCQueue<int> * buf) {
    return *buf->front();
}

int UndoRedoCircularBuffer::back(rigtorp::SPSCQueue<int> * buf) {
    size_t size = buf->size();
    rigtorp::SPSCQueue<int> tmp(size);
    int back = 0;
    for (int i = 0; i < size; i++) {
        int value = *buf->front();
        if (i == size-1) back = value;
        tmp.push(value);
        buf->pop();
    }
    for (int i = 0; i < size; i++) {
        buf->push(*tmp.front());
        tmp.pop();
    }
    return back;
}

UndoRedoCircularBuffer::Command::Command(int c, int d) {
    cmd = c;
    data = d;
}

UndoRedoCircularBuffer::Command UndoRedoCircularBuffer::push_front(rigtorp::SPSCQueue<int> * buf, int cmd, int data) {
    push_front(buf, cmd);
    push_front(buf, data);
    return {cmd, data};
}

UndoRedoCircularBuffer::Command UndoRedoCircularBuffer::pop_back_(rigtorp::SPSCQueue<int> * buf) {
    int data = pop_back(buf);
    int cmd = pop_back(buf);
    return {cmd, data};
}

void UndoRedoCircularBuffer::add(int n) const {
    if (DEBUG_COMMANDS) LOG_MAGNUM_DEBUG << "add " << n << std::endl;
    if (main->size() == main->capacity()) {
        push_front(undo_, ADD_WRAPPED, front());
    } else {
        push_front(undo_, ADD, n);
    }
    push_front(main, n);
}

int UndoRedoCircularBuffer::remove() const {
    if (DEBUG_COMMANDS) LOG_MAGNUM_DEBUG << "remove" << std::endl;
    int * ptr = main->front();
    if (ptr == nullptr) return 0;
    int r = *ptr;
    main->pop();
    push_front(undo_, REMOVE);
    push_front(undo_, r);
    return r;
}

void UndoRedoCircularBuffer::undo() const {
    if (undo_->empty()) return;
    Command command = pop_back_(undo_);
    switch (command.cmd) {
        case ADD: {
            LOG_MAGNUM_DEBUG << "undo ADD" << std::endl;
            push_front(redo_, command.cmd, pop_back(main));
            break;
        }
        case ADD_WRAPPED: {
            LOG_MAGNUM_DEBUG << "undo ADD_WRAPPED" << std::endl;
            int value = back(main);
            push_back(main, command.data);
            push_front(redo_, command.cmd, value);
            break;
        }
        case REMOVE: {
            LOG_MAGNUM_DEBUG << "undo REMOVE" << std::endl;
            push_back(main, command.data);
            push_front(redo_, command.cmd, command.data);
            break;
        }
        default: {
            LOG_MAGNUM_FATAL << "undo UNKNOWN COMMAND" << std::endl;
        }
    }
}

void UndoRedoCircularBuffer::redo() const {
    if (redo_->empty()) return;
    Command command = pop_back_(redo_);
    switch (command.cmd) {
        case ADD: {
            LOG_MAGNUM_DEBUG << "redo ADD" << std::endl;
            push_front(undo_, command.cmd, command.data);
            push_front(main, command.data);
            break;
        }
        case ADD_WRAPPED: {
            LOG_MAGNUM_DEBUG << "redo ADD_WRAPPED" << std::endl;
            push_front(undo_, command.cmd, front(main));
            push_front(main, command.data);
            break;
        }
        case REMOVE: {
            LOG_MAGNUM_DEBUG << "redo REMOVE" << std::endl;
            push_front(undo_, command.cmd, command.data);
            main->pop();
            break;
        }
        default: {
            LOG_MAGNUM_FATAL << "redo UNKNOWN COMMAND" << std::endl;
        }
    }
}

int UndoRedoCircularBuffer::peek() const {
    if (DEBUG_COMMANDS) LOG_MAGNUM_DEBUG << "peek" << std::endl;
    int * ptr = main->front();
    if (ptr == nullptr) return 0;
    return *ptr;
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

std::string UndoRedoCircularBuffer::toString(rigtorp::SPSCQueue<int> * buf) const {
    size_t size = buf->capacity();
    rigtorp::SPSCQueue<int> tmp(size*2);
    std::string string = format("(size = %02zu, capacity = %02zu)", buf->size(), size);
    if (buf == main) {
        for (int i = 0; i < size; i++) {
            int *ptr = buf->front();
            int val = 0;
            tmp.push(ptr != nullptr);
            if (ptr != nullptr) {
                val = *ptr;
                buf->pop();
                string += format("%23d", val);
            } else {
                string += format("%23c", '_');
            }
            if (i+1 != size) string += ",";
            tmp.push(val);
        }
    } else {
        for (int i = 0; i < size; i += 2) {
            int *command = buf->front();
            int command_cmd = 0;
            int command_value = 0;
            tmp.push(command != nullptr);
            if (command != nullptr) {
                command_cmd = *command;
                buf->pop();
                command_value = *buf->front();
                buf->pop();
                std::string cmd = cmdToString(command_cmd);
                string += format("%4s{ %11s,%3d }", " ", cmd.data(), command_value);
            } else {
                string += format("%22s_", " ");
            }
            tmp.push(command_cmd);
            tmp.push(command != nullptr);
            tmp.push(command_value);
            if (i+2 != size) string += ",";
        }
    }
    for (int i = 0; i < size; i++) {
        int shouldPush = *tmp.front();
        tmp.pop();
        int * ptr = tmp.front();
        int val = 0;
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
