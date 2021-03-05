#include <UndoRedoCircularBuffer.hpp>
#include <Log.hpp>

const int UndoRedoCircularBuffer::ADD = 1;
const int UndoRedoCircularBuffer::REMOVE = 2;

UndoRedoCircularBuffer::UndoRedoCircularBuffer(int size) : UndoRedoCircularBuffer(size, size, size) {}

UndoRedoCircularBuffer::UndoRedoCircularBuffer(int size, int undo_redo_size) : UndoRedoCircularBuffer(size, undo_redo_size, undo_redo_size) {}

UndoRedoCircularBuffer::UndoRedoCircularBuffer(int size, int undo_size, int redo_size) :
    main(new rigtorp::SPSCQueue<int>(size)),
    undo_(new rigtorp::SPSCQueue<int>(undo_size*2)),
    redo_(new rigtorp::SPSCQueue<int>(redo_size*2))
{}

UndoRedoCircularBuffer::~UndoRedoCircularBuffer() {
    delete main;
    delete undo_;
    delete redo_;
};

int UndoRedoCircularBuffer::size() const {
    return main->size();
}

/*
[ RUN      ] CircularBuffer_Core.add_multi_then_undo_multi
add 5
MAIN:
   0   0   0
UNDO:
   0   0   0   0   0   0
REDO:
   0   0   0   0   0   0

undo_->size() = 0
undo_->capacity() = 6
add - pushing undo command
add - pushing undo value
add - pushing main value
MAIN:
   5   0   0
UNDO:
   1   0   0   0   0   0
REDO:
   0   0   0   0   0   0

add 5
MAIN:
   5   0   0
UNDO:
   1   0   0   0   0   0
REDO:
   0   0   0   0   0   0

undo_->size() = 2
undo_->capacity() = 6
add - pushing undo command
add - pushing undo value
add - pushing main value
MAIN:
   5   5   0
UNDO:
   1   0   1   5   0   0
REDO:
   0   0   0   0   0   0

undo - popping undo
undo add
undo add - pushing redo command
undo add - pushing redo value
undo add - popping main
undo add - popping undo
undo - popping undo
undo add
undo add - pushing redo command
undo add - pushing redo value
undo add - popping main
undo add - popping undo
MAIN:
   0   0   0
UNDO:
   0   0   0   0   0   0
REDO:
   1   5   1   5   0   0
redo - popping redo
redo - popping redo
redo - pushing undo command
redo - pushing undo value
redo add
add 5
MAIN:
   0   0   0
UNDO:
   1   5   0   0   0   0
REDO:
   1   5   0   0   0   0

undo_->size() = 2
undo_->capacity() = 6
add - pushing undo command
add - pushing undo value
add - pushing main value
MAIN:
   5   0   0
UNDO:
   1   5   1   0   0   0
REDO:
   1   5   0   0   0   0

redo - popping redo
redo - popping redo
redo - pushing undo command
redo - pushing undo value
redo add
add 5
MAIN:
   5   0   0
UNDO:
   1   5   1   0   1   5
REDO:
   0   0   0   0   0   0

undo_->size() = 6
undo_->capacity() = 6
add - pushing undo command
 */
void UndoRedoCircularBuffer::add(int n) const {
    LOG_MAGNUM_DEBUG << "add " << n << std::endl;
    int * ptr = main->front();
    LOG_MAGNUM_DEBUG << toString() << std::endl;
    LOG_MAGNUM_DEBUG_FUNCTION(undo_->size());
    LOG_MAGNUM_DEBUG_FUNCTION(undo_->capacity());
    LOG_MAGNUM_DEBUG << "add - pushing undo command" << std::endl;
    undo_->push(ADD);
    LOG_MAGNUM_DEBUG << "add - pushing undo value" << std::endl;
    undo_->push(ptr == nullptr ? 0 : *ptr);
    LOG_MAGNUM_DEBUG << "add - pushing main value" << std::endl;
    main->push(n);
    LOG_MAGNUM_DEBUG << toString() << std::endl;
}

int UndoRedoCircularBuffer::remove() const {
    LOG_MAGNUM_DEBUG << "remove " << std::endl;
    int * ptr = main->front();
    if (ptr == nullptr) return 0;
    int r = *ptr;
    LOG_MAGNUM_DEBUG << "remove - popping main" << std::endl;
    main->pop();
    LOG_MAGNUM_DEBUG << "remove - pushing undo command" << std::endl;
    undo_->push(REMOVE);
    LOG_MAGNUM_DEBUG << "remove - popping undo value" << std::endl;
    undo_->push(r);
    return r;
}

void UndoRedoCircularBuffer::undo() const {
    int * ptr = undo_->front();
    if (ptr == nullptr) return;
    int command = *ptr;
    LOG_MAGNUM_DEBUG << "undo - popping undo" << std::endl;
    undo_->pop();
    switch (command) {
        case ADD: {
            LOG_MAGNUM_DEBUG << "undo add" << std::endl;
            int *mainptr = main->front();
            if (mainptr == nullptr) {
                LOG_MAGNUM_FATAL << "undo add - front is nullptr" << std::endl;
                return;
            }
            LOG_MAGNUM_DEBUG << "undo add - pushing redo command" << std::endl;
            redo_->push(command);
            LOG_MAGNUM_DEBUG << "undo add - pushing redo value" << std::endl;
            redo_->push(*mainptr);
            LOG_MAGNUM_DEBUG << "undo add - popping main" << std::endl;
            main->pop();
            LOG_MAGNUM_DEBUG << "undo add - popping undo" << std::endl;
            undo_->pop();
            break;
        }
        case REMOVE: {
            LOG_MAGNUM_DEBUG << "undo remove" << std::endl;
            int *undoptr = undo_->front();
            if (undoptr == nullptr) {
                LOG_MAGNUM_FATAL << "undo add - main is null" << std::endl;
                return;
            }
            int value = *undoptr;
            LOG_MAGNUM_DEBUG << "undo add - pushing redo command" << std::endl;
            redo_->push(command);
            LOG_MAGNUM_DEBUG << "undo add - pushing redo value" << std::endl;
            redo_->push(value);
            LOG_MAGNUM_DEBUG << "undo add - popping undo" << std::endl;
            undo_->pop();
            LOG_MAGNUM_DEBUG << "undo add - pushing main value" << std::endl;
            main->push(value);
            break;
        }
        default: {
            LOG_MAGNUM_FATAL << "undo UNKNOWN" << std::endl;
            break;
        }
    }
}

void UndoRedoCircularBuffer::redo() const {
    int * ptr = redo_->front();
    if (ptr == nullptr) return;
    int command = *ptr;
    LOG_MAGNUM_DEBUG << "redo - popping redo" << std::endl;
    redo_->pop();
    int * redoptr = redo_->front();
    if (redoptr == nullptr) {
        LOG_MAGNUM_FATAL << "redo - main is null" << std::endl;
        return;
    }
    int value = *redoptr;
    LOG_MAGNUM_DEBUG << "redo - popping redo" << std::endl;
    redo_->pop();
    LOG_MAGNUM_DEBUG << "redo - pushing undo command" << std::endl;
    undo_->push(command);
    LOG_MAGNUM_DEBUG << "redo - pushing undo value" << std::endl;
    undo_->push(value);
    switch (command) {
        case ADD: {
            LOG_MAGNUM_DEBUG << "redo add" << std::endl;
            add(value);
            break;
        }
        case REMOVE: {
            LOG_MAGNUM_DEBUG << "redo remove" << std::endl;
            LOG_MAGNUM_DEBUG << "redo - pushing main value" << std::endl;
            main->push(value);
            break;
        }
        default: {
            LOG_MAGNUM_FATAL << "redo UNKNOWN" << std::endl;
            break;
        }
    }
}

int UndoRedoCircularBuffer::peek() const {
    LOG_MAGNUM_DEBUG << "peek" << std::endl;
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
    std::string string = "";
    for (int i = 0; i < size; i++) {
        int * ptr = buf->front();
        int val = 0;
        tmp.push(ptr != nullptr);
        if (ptr != nullptr) {
            val = *ptr;
            buf->pop();
        }
        string += format("%4d", val);
        tmp.push(val);
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
    string += "\n";
    return string;
}

std::string UndoRedoCircularBuffer::toString() const {
    return "MAIN:\n" + toString(main) +
           "UNDO:\n" + toString(undo_) +
           "REDO:\n" + toString(redo_);
}
