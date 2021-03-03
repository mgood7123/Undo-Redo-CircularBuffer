#include <iostream>
#include <Log.hpp>

class CircularBuffer {
public:
    static void println(std::string x);

    static void test();

    template<typename ... Args>
    std::string format( const std::string& format, Args ... args );

    int CBUF_SIZE; int head, tail; int * buf;

    int CBUF_SIZE_UNDO; int head_UNDO, tail_UNDO; int * buf_UNDO;
    int CBUF_SIZE_REDO; int head_REDO, tail_REDO; int * buf_REDO;

    /**
     * @param size the buffer capacity, this is internally increased by 1
     */
    CircularBuffer(int size);

    /**
     * @param size the buffer capacity, this is internally increased by 1
     * @param undo_redo_size the undo and redo buffer capacity, this is internally increased by 1
     */
    CircularBuffer(int size, int undo_redo_size);

    /**
     * @param size the buffer capacity, this is internally increased by 1
     * @param undo_size the undo buffer capacity, this is internally increased by 1
     * @param redo_size the redo buffer capacity, this is internally increased by 1
     */
    CircularBuffer(int size, int undo_size, int redo_size);

    ~CircularBuffer();

    int size(int head, int tail, int CBUF_SIZE);

    int size();

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
     * when undoing an item:
     * 1. if head == tail, the ring buffer is empty,
     *    and so attempting to undo an item would be an error
     * 2. decrement head by one if the tail is equal to head minus 1
     * 3. set the item pointed to by the head_UNDO to its value before add took place
     * 4. Advance head_UNDO forward by 1 and modulo it with the size of the undo buffer
     * 5. if tail is 0 then set tail to buffer capacity minus 1,
     *    otherwise decrement tail by one
     * 6. get the item pointed to by tail and return it to the caller
     */
    void undo();

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

    std::string toString(int CBUF_SIZE, int head, int tail, int buffer[]);

    std::string toString();

    const size_t SIZE_OF_INT = sizeof(int);
};
