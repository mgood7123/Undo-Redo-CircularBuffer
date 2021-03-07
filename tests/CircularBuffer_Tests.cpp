//
// Created by smallville7123 on 9/08/20.
//

#include <gtest/gtest.h>
#include <UndoRedoCircularBuffer.hpp>
#include <Log.hpp>

TEST(CircularBuffer_Core, push_1_push_6_obtain_6_push_6) {
    UndoRedoCircularBuffer a(3);
    LOG_MAGNUM_DEBUG << "push 1" << std::endl;
    a.main->push(1);
    LOG_MAGNUM_DEBUG << "push 6" << std::endl;
    a.main->push(6);
    LOG_MAGNUM_DEBUG << a.toString() << std::endl;
    int front = a.front();
    int back = a.back();
    LOG_MAGNUM_DEBUG_FUNCTION(front);
    LOG_MAGNUM_DEBUG_FUNCTION(back);
    ASSERT_EQ(front, 1);
    ASSERT_EQ(back, 6);
    LOG_MAGNUM_DEBUG << a.toString() << std::endl;
    LOG_MAGNUM_DEBUG << "pop back" << std::endl;
    a.pop_back();
    LOG_MAGNUM_DEBUG << a.toString() << std::endl;
    LOG_MAGNUM_DEBUG << "push 6" << std::endl;
    a.main->push(6);
    LOG_MAGNUM_DEBUG << a.toString() << std::endl;
}

TEST(CircularBuffer_Core, add_then_undo_then_redo) {
    UndoRedoCircularBuffer a(3);
    a.add(5);
    LOG_MAGNUM_DEBUG << a.toString() << std::endl;
    a.undo();
    LOG_MAGNUM_DEBUG << a.toString() << std::endl;
    a.redo();
    LOG_MAGNUM_DEBUG << a.toString() << std::endl;
}

TEST(CircularBuffer_Core, add_multi_then_undo_multi_1) {
    UndoRedoCircularBuffer a(3);
    a.add(5);
    a.add(6);
    LOG_MAGNUM_DEBUG << a.toString() << std::endl;
    a.undo();
    LOG_MAGNUM_DEBUG << a.toString() << std::endl;
    a.redo();
    LOG_MAGNUM_DEBUG << a.toString() << std::endl;
}

TEST(CircularBuffer_Core, add_multi_then_undo_multi_2) {
    UndoRedoCircularBuffer a(3);
    a.DEBUG_COMMANDS = true;
    a.DEBUG_ACTIONS = true;
    a.DEBUG_STATE = true;
    a.add(5);
    a.add(6);
    LOG_MAGNUM_DEBUG << a.toString() << std::endl;
    a.undo();
    LOG_MAGNUM_DEBUG << a.toString() << std::endl;
    a.redo();
    LOG_MAGNUM_DEBUG << a.toString() << std::endl;
    a.undo();
    LOG_MAGNUM_DEBUG << a.toString() << std::endl;
    a.redo();
    LOG_MAGNUM_DEBUG << a.toString() << std::endl;
    a.undo();
    LOG_MAGNUM_DEBUG << a.toString() << std::endl;
    a.undo();
    LOG_MAGNUM_DEBUG << a.toString() << std::endl;
    a.redo();
    LOG_MAGNUM_DEBUG << a.toString() << std::endl;
    a.redo();
    LOG_MAGNUM_DEBUG << a.toString() << std::endl;
}

TEST(CircularBuffer_Core, wrap_around_undo) {
    UndoRedoCircularBuffer a(3, 4);
    a.add(1);
    a.add(2);
    LOG_MAGNUM_DEBUG << a.toString() << std::endl;
    a.undo();
    LOG_MAGNUM_DEBUG << a.toString() << std::endl;
    a.redo();
    LOG_MAGNUM_DEBUG << a.toString() << std::endl;
    a.undo();
    LOG_MAGNUM_DEBUG << a.toString() << std::endl;
    a.redo();
    LOG_MAGNUM_DEBUG << a.toString() << std::endl;
    a.add(3);
    a.add(4);
    LOG_MAGNUM_DEBUG << a.toString() << std::endl;
    a.undo();
    LOG_MAGNUM_DEBUG << a.toString() << std::endl;
    a.redo();
    LOG_MAGNUM_DEBUG << a.toString() << std::endl;
    a.undo();
    LOG_MAGNUM_DEBUG << a.toString() << std::endl;
    a.redo();
    LOG_MAGNUM_DEBUG << a.toString() << std::endl;
}

TEST(CircularBuffer_Core, undo_3) {
    UndoRedoCircularBuffer a(3, 6);
    a.add(1);
    LOG_MAGNUM_DEBUG << a.toString() << std::endl;
    a.add(2);
    LOG_MAGNUM_DEBUG << a.toString() << std::endl;
    a.add(3);
    LOG_MAGNUM_DEBUG << a.toString() << std::endl;
    a.add(5);
    LOG_MAGNUM_DEBUG << a.toString() << std::endl;
    a.undo();
    LOG_MAGNUM_DEBUG << a.toString() << std::endl;
    a.redo();
    LOG_MAGNUM_DEBUG << a.toString() << std::endl;
}

TEST(CircularBuffer_Core, undo_4) {
    UndoRedoCircularBuffer a(3, 5);
    a.add(1);
    a.add(2);
    a.add(3);
    a.add(4);
    a.add(5);
    LOG_MAGNUM_DEBUG << a.toString() << std::endl;
    a.undo();
    a.redo();
    a.undo();
    a.undo();
    a.undo();
    a.redo();
    a.redo();
    a.redo();
    LOG_MAGNUM_DEBUG << a.toString() << std::endl;
    a.remove();
    LOG_MAGNUM_DEBUG << a.toString() << std::endl;
    a.undo();
    LOG_MAGNUM_DEBUG << a.toString() << std::endl;
    a.redo();
    LOG_MAGNUM_DEBUG << a.toString() << std::endl;
}