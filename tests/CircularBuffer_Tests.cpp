//
// Created by smallville7123 on 9/08/20.
//

#include <gtest/gtest.h>
#include <UndoRedoCircularBuffer.hpp>
#include <Log.hpp>

TEST(CircularBuffer_Core, add_then_undo_then_redo) {
    UndoRedoCircularBuffer a(3);
    a.add(5);
    LOG_MAGNUM_DEBUG << a.toString();
    a.undo();
    LOG_MAGNUM_DEBUG << a.toString();
    a.redo();
    LOG_MAGNUM_DEBUG << a.toString();
}

TEST(CircularBuffer_Core, add_multi_then_undo_multi) {
    UndoRedoCircularBuffer a(3);
    a.add(5);
    a.add(5);
    a.undo();
    a.undo();
    LOG_MAGNUM_DEBUG << a.toString();
    a.redo();
    a.redo();
    LOG_MAGNUM_DEBUG << a.toString();
    a.undo();
    a.undo();
    LOG_MAGNUM_DEBUG << a.toString();
    a.redo();
    a.redo();
    LOG_MAGNUM_DEBUG << a.toString();
}

//TEST(CircularBuffer_Core, wrap_around_undo) {
//    UndoRedoCircularBuffer a(3);
//    a.add(1);
//    a.add(2);
//    a.add(3);
//    a.add(4);
//    LOG_MAGNUM_DEBUG << a.toString();
//    a.undo();
//    LOG_MAGNUM_DEBUG << a.toString();
//}