//
// Created by smallville7123 on 9/08/20.
//

#include <gtest/gtest.h>
#include <UndoRedoCircularBuffer.hpp>
#include <Log.hpp>

TEST(MinimalArray, Core) {
    MinimalArray<int> a(5);
    ASSERT_EQ(a.size(), 5);
    ASSERT_EQ(a.is_allocated(), true);
    a[2] = 5;
    ASSERT_EQ(a[2], 5);
    MinimalArray<int> b = std::move(a);
    ASSERT_EQ(a.size(), 0);
    ASSERT_EQ(a.is_allocated(), false);
    ASSERT_EQ(b.size(), 5);
    ASSERT_EQ(b.is_allocated(), true);
}

TEST(CircularBuffer_Internal, Core) {
    UndoRedoCircularBuffer a(3);
    a.main->push(1);
    a.main->push(6);
    auto array = a.toArray();
    ASSERT_EQ(array.size(), 2);
    ASSERT_EQ(array[0], 1);
    ASSERT_EQ(array[1], 6);
    int front = a.front();
    int back = a.back();
    ASSERT_EQ(front, 1);
    ASSERT_EQ(back, 6);
    a.pop_back__(a.main);
    array = a.toArray();
    ASSERT_EQ(array.size(), 1);
    ASSERT_EQ(array[0], 1);
    a.main->push(6);
    array = a.toArray();
    ASSERT_EQ(array.size(), 2);
    ASSERT_EQ(array[0], 1);
    ASSERT_EQ(array[1], 6);
}

TEST(CircularBuffer_Undo_Redo, add) {
    UndoRedoCircularBuffer a(3);
    a.push_front(5);
    auto array = a.toArray();
    ASSERT_EQ(array.size(), 1);
    ASSERT_EQ(array[0], 5);
    a.undo();
    ASSERT_EQ(a.size(), 0);
    a.redo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 1);
    ASSERT_EQ(array[0], 5);
}

TEST(CircularBuffer_Undo_Redo, push_front) {
    UndoRedoCircularBuffer a(3);
    a.push_front(5);
    a.push_front(6);
    auto array = a.toArray();
    ASSERT_EQ(array.size(), 2);
    ASSERT_EQ(array[0], 6);
    ASSERT_EQ(array[1], 5);
    a.undo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 1);
    ASSERT_EQ(array[0], 5);
    a.redo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 2);
    ASSERT_EQ(array[0], 6);
    ASSERT_EQ(array[1], 5);
}

TEST(CircularBuffer_Undo_Redo, push_back) {
    UndoRedoCircularBuffer a(3);
    a.push_back(5);
    a.push_back(6);
    auto array = a.toArray();
    ASSERT_EQ(array.size(), 2);
    ASSERT_EQ(array[0], 5);
    ASSERT_EQ(array[1], 6);
    a.undo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 1);
    ASSERT_EQ(array[0], 5);
    a.redo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 2);
    ASSERT_EQ(array[0], 5);
    ASSERT_EQ(array[1], 6);
}

TEST(CircularBuffer_Undo_Redo, push_front_2) {
    UndoRedoCircularBuffer a(3);
    a.push_front(5);
    a.push_front(6);
    auto array = a.toArray();
    ASSERT_EQ(array.size(), 2);
    ASSERT_EQ(array[0], 6);
    ASSERT_EQ(array[1], 5);
    a.undo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 1);
    ASSERT_EQ(array[0], 5);
    a.redo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 2);
    ASSERT_EQ(array[0], 6);
    ASSERT_EQ(array[1], 5);
    a.undo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 1);
    ASSERT_EQ(array[0], 5);
    a.redo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 2);
    ASSERT_EQ(array[0], 6);
    ASSERT_EQ(array[1], 5);
    a.undo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 1);
    ASSERT_EQ(array[0], 5);
    a.undo();
    ASSERT_EQ(a.size(), 0);
    a.redo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 1);
    ASSERT_EQ(array[0], 5);
    a.redo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 2);
    ASSERT_EQ(array[0], 6);
    ASSERT_EQ(array[1], 5);
}

TEST(CircularBuffer_Undo_Redo, push_back_2) {
    UndoRedoCircularBuffer a(3);
    a.push_back(5);
    a.push_back(6);
    auto array = a.toArray();
    ASSERT_EQ(array.size(), 2);
    ASSERT_EQ(array[0], 5);
    ASSERT_EQ(array[1], 6);
    a.undo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 1);
    ASSERT_EQ(array[0], 5);
    a.redo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 2);
    ASSERT_EQ(array[0], 5);
    ASSERT_EQ(array[1], 6);
    a.undo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 1);
    ASSERT_EQ(array[0], 5);
    a.redo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 2);
    ASSERT_EQ(array[0], 5);
    ASSERT_EQ(array[1], 6);
    a.undo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 1);
    ASSERT_EQ(array[0], 5);
    a.undo();
    ASSERT_EQ(a.size(), 0);
    a.redo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 1);
    ASSERT_EQ(array[0], 5);
    a.redo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 2);
    ASSERT_EQ(array[0], 5);
    ASSERT_EQ(array[1], 6);
}

TEST(CircularBuffer_Undo_Redo_Wrap, push_front) {
    UndoRedoCircularBuffer a(3, 4);
    a.push_front(1);
    a.push_front(2);
    auto array = a.toArray();
    ASSERT_EQ(array.size(), 2);
    ASSERT_EQ(array[0], 2);
    ASSERT_EQ(array[1], 1);
    a.undo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 1);
    ASSERT_EQ(array[0], 1);
    a.redo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 2);
    ASSERT_EQ(array[0], 2);
    ASSERT_EQ(array[1], 1);
    a.undo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 1);
    ASSERT_EQ(array[0], 1);
    a.redo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 2);
    ASSERT_EQ(array[0], 2);
    ASSERT_EQ(array[1], 1);
    a.push_front(3);
    array = a.toArray();
    ASSERT_EQ(array.size(), 3);
    ASSERT_EQ(array[0], 3);
    ASSERT_EQ(array[1], 2);
    ASSERT_EQ(array[2], 1);
    a.push_front(4);
    array = a.toArray();
    ASSERT_EQ(array.size(), 3);
    ASSERT_EQ(array[0], 4);
    ASSERT_EQ(array[1], 3);
    ASSERT_EQ(array[2], 2);
    a.undo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 3);
    ASSERT_EQ(array[0], 3);
    ASSERT_EQ(array[1], 2);
    ASSERT_EQ(array[2], 1);
    a.redo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 3);
    ASSERT_EQ(array[0], 4);
    ASSERT_EQ(array[1], 3);
    ASSERT_EQ(array[2], 2);
    a.undo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 3);
    ASSERT_EQ(array[0], 3);
    ASSERT_EQ(array[1], 2);
    ASSERT_EQ(array[2], 1);
    a.redo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 3);
    ASSERT_EQ(array[0], 4);
    ASSERT_EQ(array[1], 3);
    ASSERT_EQ(array[2], 2);
}

TEST(CircularBuffer_Undo_Redo_Wrap, push_back) {
    UndoRedoCircularBuffer a(3, 4);
    a.push_back(1);
    a.push_back(2);
    auto array = a.toArray();
    ASSERT_EQ(array.size(), 2);
    ASSERT_EQ(array[0], 1);
    ASSERT_EQ(array[1], 2);
    a.undo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 1);
    ASSERT_EQ(array[0], 1);
    a.redo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 2);
    ASSERT_EQ(array[0], 1);
    ASSERT_EQ(array[1], 2);
    a.undo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 1);
    ASSERT_EQ(array[0], 1);
    a.redo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 2);
    ASSERT_EQ(array[0], 1);
    ASSERT_EQ(array[1], 2);
    a.push_back(3);
    a.push_back(4);
    array = a.toArray();
    ASSERT_EQ(array.size(), 3);
    ASSERT_EQ(array[0], 2);
    ASSERT_EQ(array[1], 3);
    ASSERT_EQ(array[2], 4);
    a.undo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 3);
    ASSERT_EQ(array[0], 1);
    ASSERT_EQ(array[1], 2);
    ASSERT_EQ(array[2], 3);
    a.redo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 3);
    ASSERT_EQ(array[0], 2);
    ASSERT_EQ(array[1], 3);
    ASSERT_EQ(array[2], 4);
    a.undo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 3);
    ASSERT_EQ(array[0], 1);
    ASSERT_EQ(array[1], 2);
    ASSERT_EQ(array[2], 3);
    a.redo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 3);
    ASSERT_EQ(array[0], 2);
    ASSERT_EQ(array[1], 3);
    ASSERT_EQ(array[2], 4);
}

TEST(CircularBuffer_Undo_Redo_Wrap, push_front_2) {
    UndoRedoCircularBuffer a(3, 6);
    a.push_front(1);
    auto array = a.toArray();
    ASSERT_EQ(array.size(), 1);
    ASSERT_EQ(array[0], 1);
    a.push_front(2);
    array = a.toArray();
    ASSERT_EQ(array.size(), 2);
    ASSERT_EQ(array[0], 2);
    ASSERT_EQ(array[1], 1);
    a.push_front(3);
    array = a.toArray();
    ASSERT_EQ(array.size(), 3);
    ASSERT_EQ(array[0], 3);
    ASSERT_EQ(array[1], 2);
    ASSERT_EQ(array[2], 1);
    a.push_front(5);
    array = a.toArray();
    ASSERT_EQ(array.size(), 3);
    ASSERT_EQ(array[0], 5);
    ASSERT_EQ(array[1], 3);
    ASSERT_EQ(array[2], 2);
    a.undo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 3);
    ASSERT_EQ(array[0], 3);
    ASSERT_EQ(array[1], 2);
    ASSERT_EQ(array[2], 1);
    a.redo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 3);
    ASSERT_EQ(array[0], 5);
    ASSERT_EQ(array[1], 3);
    ASSERT_EQ(array[2], 2);
}

TEST(CircularBuffer_Undo_Redo_Wrap, push_back_2) {
    UndoRedoCircularBuffer a(3, 6);
    a.push_back(1);
    auto array = a.toArray();
    ASSERT_EQ(array.size(), 1);
    ASSERT_EQ(array[0], 1);
    a.push_back(2);
    array = a.toArray();
    ASSERT_EQ(array.size(), 2);
    ASSERT_EQ(array[0], 1);
    ASSERT_EQ(array[1], 2);
    a.push_back(3);
    array = a.toArray();
    ASSERT_EQ(array.size(), 3);
    ASSERT_EQ(array[0], 1);
    ASSERT_EQ(array[1], 2);
    ASSERT_EQ(array[2], 3);
    a.push_back(5);
    array = a.toArray();
    ASSERT_EQ(array.size(), 3);
    ASSERT_EQ(array[0], 2);
    ASSERT_EQ(array[1], 3);
    ASSERT_EQ(array[2], 5);
    a.undo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 3);
    ASSERT_EQ(array[0], 1);
    ASSERT_EQ(array[1], 2);
    ASSERT_EQ(array[2], 3);
    a.redo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 3);
    ASSERT_EQ(array[0], 2);
    ASSERT_EQ(array[1], 3);
    ASSERT_EQ(array[2], 5);
}

TEST(CircularBuffer_Undo_Redo_Wrap, push_front_3) {
    UndoRedoCircularBuffer a(3, 5);
    a.push_front(1);
    auto array = a.toArray();
    ASSERT_EQ(array.size(), 1);
    ASSERT_EQ(array[0], 1);
    a.push_front(2);
    array = a.toArray();
    ASSERT_EQ(array.size(), 2);
    ASSERT_EQ(array[0], 2);
    ASSERT_EQ(array[1], 1);
    a.push_front(3);
    array = a.toArray();
    ASSERT_EQ(array.size(), 3);
    ASSERT_EQ(array[0], 3);
    ASSERT_EQ(array[1], 2);
    ASSERT_EQ(array[2], 1);
    a.push_front(4);
    array = a.toArray();
    ASSERT_EQ(array.size(), 3);
    ASSERT_EQ(array[0], 4);
    ASSERT_EQ(array[1], 3);
    ASSERT_EQ(array[2], 2);
    a.push_front(5);
    array = a.toArray();
    ASSERT_EQ(array.size(), 3);
    ASSERT_EQ(array[0], 5);
    ASSERT_EQ(array[1], 4);
    ASSERT_EQ(array[2], 3);
    a.undo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 3);
    ASSERT_EQ(array[0], 4);
    ASSERT_EQ(array[1], 3);
    ASSERT_EQ(array[2], 2);
    a.redo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 3);
    ASSERT_EQ(array[0], 5);
    ASSERT_EQ(array[1], 4);
    ASSERT_EQ(array[2], 3);
    a.undo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 3);
    ASSERT_EQ(array[0], 4);
    ASSERT_EQ(array[1], 3);
    ASSERT_EQ(array[2], 2);
    a.undo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 3);
    ASSERT_EQ(array[0], 3);
    ASSERT_EQ(array[1], 2);
    ASSERT_EQ(array[2], 1);
    a.undo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 2);
    ASSERT_EQ(array[0], 2);
    ASSERT_EQ(array[1], 1);
    a.redo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 3);
    ASSERT_EQ(array[0], 3);
    ASSERT_EQ(array[1], 2);
    ASSERT_EQ(array[2], 1);
    a.redo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 3);
    ASSERT_EQ(array[0], 4);
    ASSERT_EQ(array[1], 3);
    ASSERT_EQ(array[2], 2);
    a.redo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 3);
    ASSERT_EQ(array[0], 5);
    ASSERT_EQ(array[1], 4);
    ASSERT_EQ(array[2], 3);
    a.pop_front();
    array = a.toArray();
    ASSERT_EQ(array.size(), 2);
    ASSERT_EQ(array[0], 4);
    ASSERT_EQ(array[1], 3);
    a.undo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 3);
    ASSERT_EQ(array[0], 5);
    ASSERT_EQ(array[1], 4);
    ASSERT_EQ(array[2], 3);
    a.redo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 2);
    ASSERT_EQ(array[0], 4);
    ASSERT_EQ(array[1], 3);
}

TEST(CircularBuffer_Undo_Redo_Wrap, push_back_3) {
    UndoRedoCircularBuffer a(3, 5);
    a.push_back(1);
    auto array = a.toArray();
    ASSERT_EQ(array.size(), 1);
    ASSERT_EQ(array[0], 1);
    a.push_back(2);
    array = a.toArray();
    ASSERT_EQ(array.size(), 2);
    ASSERT_EQ(array[0], 1);
    ASSERT_EQ(array[1], 2);
    a.push_back(3);
    array = a.toArray();
    ASSERT_EQ(array.size(), 3);
    ASSERT_EQ(array[0], 1);
    ASSERT_EQ(array[1], 2);
    ASSERT_EQ(array[2], 3);
    a.push_back(4);
    array = a.toArray();
    ASSERT_EQ(array.size(), 3);
    ASSERT_EQ(array[0], 2);
    ASSERT_EQ(array[1], 3);
    ASSERT_EQ(array[2], 4);
    a.push_back(5);
    array = a.toArray();
    ASSERT_EQ(array.size(), 3);
    ASSERT_EQ(array[0], 3);
    ASSERT_EQ(array[1], 4);
    ASSERT_EQ(array[2], 5);
    a.undo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 3);
    ASSERT_EQ(array[0], 2);
    ASSERT_EQ(array[1], 3);
    ASSERT_EQ(array[2], 4);
    a.redo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 3);
    ASSERT_EQ(array[0], 3);
    ASSERT_EQ(array[1], 4);
    ASSERT_EQ(array[2], 5);
    a.undo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 3);
    ASSERT_EQ(array[0], 2);
    ASSERT_EQ(array[1], 3);
    ASSERT_EQ(array[2], 4);
    a.undo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 3);
    ASSERT_EQ(array[0], 1);
    ASSERT_EQ(array[1], 2);
    ASSERT_EQ(array[2], 3);
    a.undo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 2);
    ASSERT_EQ(array[0], 1);
    ASSERT_EQ(array[1], 2);
    a.redo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 3);
    ASSERT_EQ(array[0], 1);
    ASSERT_EQ(array[1], 2);
    ASSERT_EQ(array[2], 3);
    a.redo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 3);
    ASSERT_EQ(array[0], 2);
    ASSERT_EQ(array[1], 3);
    ASSERT_EQ(array[2], 4);
    a.redo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 3);
    ASSERT_EQ(array[0], 3);
    ASSERT_EQ(array[1], 4);
    ASSERT_EQ(array[2], 5);
    a.pop_back();
    array = a.toArray();
    ASSERT_EQ(array.size(), 2);
    ASSERT_EQ(array[0], 3);
    ASSERT_EQ(array[1], 4);
    a.undo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 3);
    ASSERT_EQ(array[0], 3);
    ASSERT_EQ(array[1], 4);
    ASSERT_EQ(array[2], 5);
    a.redo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 2);
    ASSERT_EQ(array[0], 3);
    ASSERT_EQ(array[1], 4);
    a.undo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 3);
    ASSERT_EQ(array[0], 3);
    ASSERT_EQ(array[1], 4);
    ASSERT_EQ(array[2], 5);
    a.redo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 2);
    ASSERT_EQ(array[0], 3);
    ASSERT_EQ(array[1], 4);
    a.undo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 3);
    ASSERT_EQ(array[0], 3);
    ASSERT_EQ(array[1], 4);
    ASSERT_EQ(array[2], 5);
    a.pop_front();
    array = a.toArray();
    ASSERT_EQ(array.size(), 2);
    ASSERT_EQ(array[0], 4);
    ASSERT_EQ(array[1], 5);
    a.undo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 3);
    ASSERT_EQ(array[0], 3);
    ASSERT_EQ(array[1], 4);
    ASSERT_EQ(array[2], 5);
    a.redo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 2);
    ASSERT_EQ(array[0], 4);
    ASSERT_EQ(array[1], 5);
    a.undo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 3);
    ASSERT_EQ(array[0], 3);
    ASSERT_EQ(array[1], 4);
    ASSERT_EQ(array[2], 5);
    a.redo();
    array = a.toArray();
    ASSERT_EQ(array.size(), 2);
    ASSERT_EQ(array[0], 4);
    ASSERT_EQ(array[1], 5);
}