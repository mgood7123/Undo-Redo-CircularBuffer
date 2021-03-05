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
//    LOG_MAGNUM_INFO << "adding " << n << std::endl;

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
//    LOG_MAGNUM_INFO << "undo" << std::endl;

    if (main_head == main_tail) {
        return;
    }

    if (undo_tail == undo_head - 1) {
//        LOG_MAGNUM_DEBUG << 1 << std::endl;
        buf[main_tail-1] = buf[undo_tail];
        undo_tail = undo_tail == undo_offset ? (redo_offset-1) : undo_tail - 1;
        undo_head--;
        main_head--;
        if (main_tail == 0) main_tail = main_capacity-1;
        else main_tail--;
    } else if (undo_head == undo_offset) {
        /*
restore to
    main:
       h       t
       5   6   0   (2)

    undo:
       h   t
       5   0   0   (1)
current
    main:
       t   h
       5   6   7   (2)

    undo:
       h       t
       5   6   0   (2)
         */
//        LOG_MAGNUM_DEBUG << 2 << std::endl;
        // restore undo head and tail
        if (main_head != main_tail - 1) {
//            LOG_MAGNUM_DEBUG << 3 << std::endl;
            undo_tail = undo_tail == undo_offset ? (redo_offset - 1) : (undo_tail - 1);
            if (undo_head != undo_offset) undo_head--;
            // restore tail value
            buf[main_tail] = 0;
            // head value does not need to be restored
        }
        /*
main:
   h   t
   5   0   0   (1)

undo:
   h
   0   0   0   (0)
        */
        if (main_head == 0) {
//            LOG_MAGNUM_DEBUG << 4 << std::endl;
            if (main_tail == 1) {
//                LOG_MAGNUM_DEBUG << 5 << std::endl;
                // restore tail
                main_tail = 0;
                // restore tail value
                buf[main_tail] = 0;
                buf[undo_tail] = 0;
                // head value does not need to be restored
            } else if (main_tail == undo_offset-1) {
//                LOG_MAGNUM_DEBUG << 6 << std::endl;
//                LOG_MAGNUM_DEBUG << toString() << std::endl;
                /*
        target
        main:
           h       t
           5   6   0   (2)

        undo:
           h   t
           5   0   0   (1)
                 */
                if (main_head == 1) {
                    /*
        current
        main:
           t   h
           5   6   7   (2)

        undo:
           h       t
           5   6   0   (2)
                     */
                    buf[main_head + 1] = buf[undo_tail];
                    main_head--;
                    main_tail = undo_offset - 1;
                    undo_tail--;
                    // restore tail value
                    buf[main_tail] = 0;
                    // head value does not need to be restored
                } else {
                    /*
        current
        main:
           h       t
           5   6   0   (2)

        undo:
           h
           5   6   0   (0)
                    */
                    buf[undo_head + 1] = 0;
                    main_tail--;
                    buf[main_tail] = 0;
                }
            } else {
//                LOG_MAGNUM_DEBUG << 7 << std::endl;
                // restore head
                main_head--;
                // restore tail
                main_tail = main_tail == 0 ? (undo_offset - 1) : (main_tail - 1);
                // restore tail value
                buf[main_tail] = 0;
                // head value does not need to be restored
            }
        } else {
//            LOG_MAGNUM_DEBUG << 12 << std::endl;
//            LOG_MAGNUM_DEBUG << toString() << std::endl;
            if (main_tail == main_capacity) {
//                LOG_MAGNUM_DEBUG << 6 << std::endl;
                /*
    target
    main:
       h       t
       5   6   0   (2)

    undo:
       h   t
       5   0   0   (1)

    current
    main:
       t   h
       0   6   7   (2)

    undo:
       h   t
       5   6   0   (1)

                 */
                buf[main_head + 1] = buf[undo_tail + 1];
                main_head--;
                main_tail = undo_offset - 1;
            } else if (main_tail == 0) {
                buf[main_tail] = buf[undo_head];
                main_head--;
                main_tail = undo_offset - 1;
                buf[main_tail] = 0;
            } else {
//                LOG_MAGNUM_DEBUG << 8 << std::endl;
                // restore tail value
                buf[main_tail] = 0;
                // head value does not need to be restored
                main_tail--;
            }
        }
    } else if (undo_tail != undo_offset) {
//        LOG_MAGNUM_DEBUG << 3 << std::endl;
        if (undo_tail != undo_offset && undo_head == undo_tail) undo_head--;
        undo_tail--;
        buf[undo_tail] = 0;
        if (main_tail == 0) main_tail = main_capacity-1;
        else main_tail--;
        buf[main_tail] = 0;
    }
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
