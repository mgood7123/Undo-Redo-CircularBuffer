#include <CircularBuffer.hpp>
#include <Log.hpp>

CircularBuffer::CircularBuffer(int size) : CircularBuffer(size, size, size) {}

CircularBuffer::CircularBuffer(int size, int undo_redo_size) : CircularBuffer(size, undo_redo_size, undo_redo_size) {}

CircularBuffer::CircularBuffer(int size, int undo_size, int redo_size) :
    main_capacity(size),
    undo_capacity(undo_size),
    redo_capacity(redo_size),
    main_head(0),
    main_tail(0)
{
    total_capacity = main_capacity + undo_capacity + redo_capacity;
    buf = std::move(std::vector<int>(total_capacity));
    undo_offset = main_capacity;
    redo_offset = undo_offset + undo_capacity;
    undo_head = undo_offset;
    undo_tail = undo_offset;
    redo_head = redo_offset;
    redo_tail = redo_offset;
    LOG_MAGNUM_INFO <<
            "CircularBuffer initialized with capacity of " << size << " plus 1 for the tail marker" << std::endl <<
            "CircularBuffer initialized undo with capacity of " << undo_size << " plus 1 for the tail marker" << std::endl <<
            "CircularBuffer initialized redo with capacity of " << redo_size << " plus 1 for the tail marker" << std::endl;
}

CircularBuffer::~CircularBuffer() {
}

int CircularBuffer::size(int head, int tail, int main_capacity) {
    return head <= tail
           ? tail - head
           : main_capacity - (head - tail);
}

int CircularBuffer::size() {
    return size(main_head, main_tail, main_capacity);
}

void CircularBuffer::add(int n) {
    LOG_MAGNUM_INFO << "adding " << n << std::endl;

    if (main_head != main_tail) {
        if (main_tail == 0) {
            buf[(undo_tail++)] = buf[main_capacity-1];
        } else {
            buf[(undo_tail++)] = buf[main_tail-1];
        }
        undo_tail = undo_tail == (redo_offset) ? undo_offset : undo_tail;
        if (undo_tail == undo_head) undo_head++;
    }

    buf[main_tail++] = n;
    main_tail = main_tail == main_capacity ? 0 : main_tail;
    if (main_tail == main_head) {
        main_head++;
        main_head = main_head == main_capacity ? 0 : main_head;
    }
}

void CircularBuffer::undo() {
    LOG_MAGNUM_INFO << "undo" << std::endl;

    if (main_head == main_tail) {
        return;
    }

    if (main_tail == main_head - 1) {
        main_head--;
        buf[main_head] = buf[undo_head];
        buf[undo_head] = 0;
        undo_head++;
        undo_head = undo_head == (redo_offset) ? undo_offset : undo_head;
    } else {
        if (undo_tail == undo_offset) {}//undo_tail = redo_offset-1;
        else {
            if (undo_tail != undo_offset && undo_head == undo_tail) undo_head--;
            undo_tail--;
            buf[undo_tail] = 0;
        }
        // what should we do here?
        //
        // main_head--;
        // buf[main_head] = buf[undo_head];
        // buf[undo_head] = 0;
        // undo_head++;
        // undo_head = undo_head == (redo_offset) ? undo_offset : undo_head;
    }
    if (main_tail == 0) main_tail = main_capacity-1;
    else main_tail--;
    buf[main_tail] = 0; // for illustration
}

int CircularBuffer::peek() {
    if (main_head == main_tail) {
        LOG_MAGNUM_INFO << "queue empty, oh noes" << std::endl;
        return 0;
    }

    int n = buf[main_head];
    LOG_MAGNUM_INFO << "peek " << n << std::endl;
    return n;
}

int CircularBuffer::remove() {
    if (main_head == main_tail) {
        LOG_MAGNUM_INFO << "queue empty, oh noes" << std::endl;
        return 0;
    }

    int n = buf[main_head];
    buf[main_head] = 0; // for illustration
    main_head++;
    main_head = main_head == main_capacity ? 0 : main_head;
    LOG_MAGNUM_INFO << "removed " << n << std::endl;
    return n;
}

template<typename... Args>
std::string CircularBuffer::format(const std::string &format, Args... args) {
    int size = snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
    if( size <= 0 ) {
        throw std::runtime_error("Error during formatting.");
    }
    std::unique_ptr<char[]> buf( new char[ size ] );
    snprintf( buf.get(), size, format.c_str(), args ... );
    return std::string( buf.get(), buf.get() + size - 1 );
}

std::string CircularBuffer::toString(int offset, int capacity, int head, int tail, std::vector<int> & buffer) {
    std::string buf = "";
    for (int i = offset; i < (offset + capacity); i++) {
        if (head == i) buf += format("%4c", 'h');
        else if (tail == i) buf += format("%4c", 't');
        else buf += format("%4c", ' ');
    }
    buf += "\n";

    for (int i = offset; i < (offset + capacity); i++) {
        buf += format("%4d", buffer[i]);
    }

    buf += format("   (%d)", size(head, tail, capacity));
    buf += "\n";
    return buf;
}

std::string CircularBuffer::toString() {
    return "main:\n" + toString(0, main_capacity, main_head, main_tail, buf) + "\n" +
    "undo:\n" + toString(undo_offset, undo_capacity, undo_head, undo_tail, buf) + "\n" +
    "redo:\n" + toString(redo_offset, redo_capacity, redo_head, redo_tail, buf) + "\n";
}
