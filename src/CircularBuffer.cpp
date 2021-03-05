#include <CircularBuffer.hpp>
#include <Log.hpp>

CircularBuffer::CircularBuffer(int size) :
    main_capacity(size),
    main_head(0),
    main_tail(0),
    buf(new int[size])
{
    memset(buf, 0, size * SIZE_OF_INT);
    LOG_MAGNUM_INFO << "CircularBuffer initialized with capacity of " << size << " plus 1 for the tail marker" << std::endl;
}

CircularBuffer::~CircularBuffer() {
    delete[] buf;
}

int CircularBuffer::size() const {
    return main_head == main_tail + 1 ?
        main_capacity :
        main_head <= main_tail ?
            main_tail - main_head :
            main_capacity - (main_head - main_tail);
}

void CircularBuffer::add(int n) {
    buf[main_tail++] = n;
    main_tail = main_tail == main_capacity ? 0 : main_tail;
    if (main_tail == main_head) {
        main_head++;
        main_head = main_head == main_capacity ? 0 : main_head;
    }
}

int CircularBuffer::peek() {
    if (main_head == main_tail) {
        LOG_MAGNUM_INFO << "queue empty, oh noes" << std::endl;
        return 0;
    }

    return buf[main_head];
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

std::string CircularBuffer::toString(int offset, int capacity, int head, int tail, int * buffer) {
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

    buf += format("   (%d)", size());
    buf += "\n";
    return buf;
}

std::string CircularBuffer::toString() {
    return toString(0, main_capacity, main_head, main_tail, buf);
}
