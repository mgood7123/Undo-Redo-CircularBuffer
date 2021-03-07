#include <vector>
#include <rigtorp/SPSCQueue.h>

#ifndef UNDO_REDO_CIRCULAR_BUFFER_H
#define UNDO_REDO_CIRCULAR_BUFFER_H


class UndoRedoCircularBuffer {
public:

    typedef long URCB_T;

    bool DEBUG_COMMANDS;
    bool DEBUG_ACTIONS;
    bool DEBUG_STATE;

    rigtorp::SPSCQueue<URCB_T> * main;
    rigtorp::SPSCQueue<URCB_T> * undo_;
    rigtorp::SPSCQueue<URCB_T> * redo_;

    static const URCB_T ADD;
    static const URCB_T ADD_WRAPPED;
    static const URCB_T REMOVE;

    /**
     * @param size the buffer capacity
     */
    UndoRedoCircularBuffer(size_t size);

    /**
     * @param size the buffer capacity
     * @param undo_redo_size the undo and redo buffer capacity
     */
    UndoRedoCircularBuffer(size_t size, size_t undo_redo_size);

    /**
     * @param size the buffer capacity
     * @param undo_size the undo buffer capacity
     * @param redo_size the redo buffer capacity
     */
    UndoRedoCircularBuffer(size_t size, size_t undo_size, size_t redo_size);

    ~UndoRedoCircularBuffer();

    size_t size() const;

    static void push_front(rigtorp::SPSCQueue<URCB_T> * buf, const URCB_T & value);
    static void push_back(rigtorp::SPSCQueue<URCB_T> * buf, const URCB_T & value);

    URCB_T front() const;
    URCB_T back() const;
    URCB_T pop_front() const;
    URCB_T pop_back() const;

    static URCB_T front(rigtorp::SPSCQueue<URCB_T> * buf);
    static URCB_T back(rigtorp::SPSCQueue<URCB_T> * buf);
    static URCB_T pop_front(rigtorp::SPSCQueue<URCB_T> * buf) ;
    static URCB_T pop_back(rigtorp::SPSCQueue<URCB_T> * buf) ;

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
    void add(URCB_T n) const;

    void undo() const;
    void redo() const;

    /**
     * when peeking an item:
     * 1. if head == tail, the ring buffer is empty,
     *    and so attempting to vectorue an item would be an error
     * 2. get the item pointed to by head and return it to the caller
     */
    URCB_T peek() const;

    /**
     * when removing an item:
     * 1. if head == tail, the ring buffer is empty,
     *    and so attempting to vectorue an item would be an error
     * 2. get the item pointed to by head and return it to the caller
     * 3. increment head by one and modulo it with the buffer size,
     *    advances it to the next item in the queue
     */
    URCB_T remove() const;

    template<typename ... Args> std::string format( const std::string & format, Args ... args ) const;

    std::string toString(rigtorp::SPSCQueue<URCB_T> * buf) const;

    std::string toString() const;

    struct Command {
        int cmd, data;
        Command(URCB_T c, URCB_T d);
    };

    static Command push_front(rigtorp::SPSCQueue<URCB_T> *buf, URCB_T cmd, URCB_T data);

    static Command pop_back_(rigtorp::SPSCQueue<URCB_T> *buf);
};
#endif // UNDO_REDO_CIRCULAR_BUFFER_H