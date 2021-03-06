#include <vector>
#include <rigtorp/SPSCQueue.h>

#ifndef UNDO_REDO_CIRCULAR_BUFFER_H
#define UNDO_REDO_CIRCULAR_BUFFER_H

class UndoRedoCircularBuffer {
public:

    bool DEBUG_COMMANDS;
    bool DEBUG_ACTIONS;
    bool DEBUG_STATE;

    rigtorp::SPSCQueue<int> * main;
    rigtorp::SPSCQueue<int> * undo_;
    rigtorp::SPSCQueue<int> * redo_;

    static const int ADD;
    static const int ADD_WRAPPED;
    static const int REMOVE;
    static const int REMOVE_WRAPPED;

    /**
     * @param size the buffer capacity
     */
    UndoRedoCircularBuffer(int size);

    /**
     * @param size the buffer capacity, this is internally increased by 1
     * @param undo_redo_size the undo and redo buffer capacity, this is internally increased by 1
     */
    UndoRedoCircularBuffer(int size, int undo_redo_size);

    /**
     * @param size the buffer capacity, this is internally increased by 1
     * @param undo_size the undo buffer capacity, this is internally increased by 1
     * @param redo_size the redo buffer capacity, this is internally increased by 1
     */
    UndoRedoCircularBuffer(int size, int undo_size, int redo_size);

    ~UndoRedoCircularBuffer();

    int size() const;

    static void push_front(rigtorp::SPSCQueue<int> * buf, const int & value);
    static void push_back(rigtorp::SPSCQueue<int> * buf, const int & value);

    int front() const;
    int back() const;
    int pop_front() const;
    int pop_back() const;

    static int front(rigtorp::SPSCQueue<int> * buf);
    static int back(rigtorp::SPSCQueue<int> * buf);
    static int pop_front(rigtorp::SPSCQueue<int> * buf) ;
    static int pop_back(rigtorp::SPSCQueue<int> * buf) ;

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
    void add(int n) const;

    void undo() const;
    void redo() const;

    /**
     * when peeking an item:
     * 1. if head == tail, the ring buffer is empty,
     *    and so attempting to vectorue an item would be an error
     * 2. get the item pointed to by head and return it to the caller
     */
    int peek() const;

    /**
     * when removing an item:
     * 1. if head == tail, the ring buffer is empty,
     *    and so attempting to vectorue an item would be an error
     * 2. get the item pointed to by head and return it to the caller
     * 3. increment head by one and modulo it with the buffer size,
     *    advances it to the next item in the queue
     */
    int remove() const;

    template<typename ... Args> std::string format( const std::string & format, Args ... args ) const;

    std::string toString(rigtorp::SPSCQueue<int> * buf) const;

    std::string toString() const;

    struct Command {
        int cmd, data;
        Command(int c, int d);
    };

    static Command push_front(rigtorp::SPSCQueue<int> *buf, int cmd, int data);

    static Command pop_back_(rigtorp::SPSCQueue<int> *buf);
};
#endif // UNDO_REDO_CIRCULAR_BUFFER_H