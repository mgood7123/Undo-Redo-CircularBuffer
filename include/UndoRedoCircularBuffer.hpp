#ifndef UNDO_REDO_CIRCULAR_BUFFER_H
#define UNDO_REDO_CIRCULAR_BUFFER_H

#include <rigtorp/SPSCQueue.h>
#include "MinimalArray.h"

class UndoRedoCircularBuffer {
public:

    typedef long URCB_T;

    bool DEBUG_COMMANDS;
    bool DEBUG_ACTIONS;
    bool DEBUG_STATE;

    void debug(bool value);
    void debug();
    void noDebug();

    rigtorp::SPSCQueue<URCB_T> * main;
    rigtorp::SPSCQueue<URCB_T> * undo_;
    rigtorp::SPSCQueue<URCB_T> * redo_;

    static const URCB_T PUSH_FRONT;
    static const URCB_T PUSH_FRONT_WRAPPED;
    static const URCB_T PUSH_BACK;
    static const URCB_T PUSH_BACK_WRAPPED;

    static const URCB_T POP_FRONT;
    static const URCB_T POP_BACK;

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
    bool empty() const;

    static void push_back(rigtorp::SPSCQueue<URCB_T> * buf, const URCB_T & value);
    static void push_front(rigtorp::SPSCQueue<URCB_T> * buf, const URCB_T & value);

    URCB_T front() const;
    URCB_T back() const;

    static URCB_T front(rigtorp::SPSCQueue<URCB_T> * buf);
    static URCB_T back(rigtorp::SPSCQueue<URCB_T> * buf);
    static URCB_T pop_front__(rigtorp::SPSCQueue<URCB_T> * buf);
    static URCB_T pop_back__(rigtorp::SPSCQueue<URCB_T> * buf);

    void push_back(URCB_T n) const;
    void push_front(URCB_T n) const;
    URCB_T pop_front() const;
    URCB_T pop_back() const;

    void undo() const;
    void redo() const;

    template<typename ... Args> std::string format( const std::string & format, Args ... args ) const;

    std::string toString(rigtorp::SPSCQueue<URCB_T> * buf) const;

    std::string toString() const;

    static MinimalArray<URCB_T>toArray(rigtorp::SPSCQueue<URCB_T> *buf) ;

    MinimalArray<URCB_T> toArray() const;

    struct Command {
        int cmd, data;
        Command(URCB_T c, URCB_T d);
    };

    static Command push_back(rigtorp::SPSCQueue<URCB_T> *buf, URCB_T cmd, URCB_T data);

    static Command pop_back_(rigtorp::SPSCQueue<URCB_T> *buf);
};
#endif // UNDO_REDO_CIRCULAR_BUFFER_H