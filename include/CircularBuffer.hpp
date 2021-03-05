#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <string>

class CircularBuffer {
public:
    template<typename ... Args>
    std::string format( const std::string & format, Args ... args );

    int * buf;

    int main_capacity;

    int main_head;
    int main_tail;

    /**
     * @param size the buffer capacity
     */
    CircularBuffer(int size);

    ~CircularBuffer();

    int size() const;

    /**
     * when adding an item:
     * 1. insert the item into the position pointed to by "tail",
     * 2. Advance tail forward by 1 and modulo it with the size of the buffer
     * 3. if the tail pointer and the head pointer are the same,
     *    advance the head pointer by 1
     *        this means that your ring buffer was full,
     *        and you need to "remove" an item to make room
     * @param n value
     */
    void add(int n);

    /**
     * when peeking an item:
     * 1. if head == tail, the ring buffer is empty,
     *    and so attempting to dequeue an item would be an error
     * 2. get the item pointed to by head and return it to the caller
     */
    int peek();

    /**
     * when removing an item:
     * 1. if head == tail, the ring buffer is empty,
     *    and so attempting to dequeue an item would be an error
     * 2. get the item pointed to by head and return it to the caller
     * 3. increment head by one and modulo it with the buffer size,
     *    advances it to the next item in the queue
     */
    int remove();

    std::string toString(int offset, int capacity, int head, int tail, int * buffer);

    std::string toString();

    constexpr static size_t SIZE_OF_INT = sizeof(int);
};
#endif // UNDO_REDO_CIRCULAR_BUFFER_H