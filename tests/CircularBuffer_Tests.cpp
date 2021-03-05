//
// Created by smallville7123 on 9/08/20.
//

#include <gtest/gtest.h>

#include <CircularBuffer.hpp>

TEST(CircularBuffer_Core, initialization_no_data_checking) {
    CircularBuffer a(3);
}

TEST(CircularBuffer_Core, initialization_data_checking) {
    CircularBuffer a(3);
    ASSERT_EQ(a.main_head, 0);
    ASSERT_EQ(a.main_tail, 0);
    ASSERT_EQ(a.undo_head, 3);
    ASSERT_EQ(a.undo_tail, 3);
    ASSERT_EQ(a.redo_head, 6);
    ASSERT_EQ(a.redo_tail, 6);
    ASSERT_EQ(a.size(), 0);
    ASSERT_EQ(a.main_capacity, 3);
    ASSERT_EQ(a.undo_capacity, 3);
    ASSERT_EQ(a.redo_capacity, 3);
    ASSERT_EQ(a.total_capacity, 9);
    ASSERT_EQ(a.buf[0], 0);
    ASSERT_EQ(a.buf[1], 0);
    ASSERT_EQ(a.buf[2], 0);
    ASSERT_EQ(a.buf[3], 0);
    ASSERT_EQ(a.buf[4], 0);
    ASSERT_EQ(a.buf[5], 0);
    ASSERT_EQ(a.buf[6], 0);
    ASSERT_EQ(a.buf[7], 0);
    ASSERT_EQ(a.buf[8], 0);
    LOG_MAGNUM_DEBUG << a.toString();
}

TEST(CircularBuffer_Core, add) {
    CircularBuffer a(3);
    a.add(5);
    ASSERT_EQ(a.main_head, 0);
    ASSERT_EQ(a.main_tail, 1);
    ASSERT_EQ(a.undo_head, 3);
    ASSERT_EQ(a.undo_tail, 3);
}

TEST(CircularBuffer_Core, add_then_undo) {
    CircularBuffer a(3);
    a.add(5);
    LOG_MAGNUM_DEBUG << a.toString();
    ASSERT_EQ(a.buf[0], 5);
    ASSERT_EQ(a.buf[1], 0);
    ASSERT_EQ(a.buf[2], 0);
    ASSERT_EQ(a.main_head, 0);
    ASSERT_EQ(a.main_tail, 1);
    ASSERT_EQ(a.undo_head, 3);
    ASSERT_EQ(a.undo_tail, 3);
    a.undo();
    LOG_MAGNUM_DEBUG << a.toString();
    ASSERT_EQ(a.buf[0], 0);
    ASSERT_EQ(a.buf[1], 0);
    ASSERT_EQ(a.buf[2], 0);
    ASSERT_EQ(a.main_head, 0);
    ASSERT_EQ(a.main_tail, 0);
    ASSERT_EQ(a.undo_head, 3);
    ASSERT_EQ(a.undo_tail, 3);
}

TEST(CircularBuffer_Core, add_multi) {
    CircularBuffer a(3);
    a.add(5);
    a.add(6);
    a.add(7);
    ASSERT_EQ(a.buf[0], 5);
    ASSERT_EQ(a.buf[1], 6);
    ASSERT_EQ(a.buf[2], 7);
    ASSERT_EQ(a.main_head, 1);
    ASSERT_EQ(a.main_tail, 0);
    ASSERT_EQ(a.undo_head, 3);
    ASSERT_EQ(a.undo_tail, 5);
}

TEST(CircularBuffer_Core, add_multi_then_undo_multi) {
    CircularBuffer a(3);
    ASSERT_EQ(a.buf[0], 0);
    ASSERT_EQ(a.buf[1], 0);
    ASSERT_EQ(a.buf[2], 0);
    ASSERT_EQ(a.main_head, 0);
    ASSERT_EQ(a.main_tail, 0);
    ASSERT_EQ(a.undo_head, 3);
    ASSERT_EQ(a.undo_tail, 3);
    a.add(5);
    ASSERT_EQ(a.buf[0], 5);
    ASSERT_EQ(a.buf[1], 0);
    ASSERT_EQ(a.buf[2], 0);
    ASSERT_EQ(a.main_head, 0);
    ASSERT_EQ(a.main_tail, 1);
    ASSERT_EQ(a.undo_head, 3);
    ASSERT_EQ(a.undo_tail, 3);
    a.add(6);
    LOG_MAGNUM_DEBUG << a.toString();
    ASSERT_EQ(a.buf[0], 5);
    ASSERT_EQ(a.buf[1], 6);
    ASSERT_EQ(a.buf[2], 0);
    ASSERT_EQ(a.main_head, 0);
    ASSERT_EQ(a.main_tail, 2);
    ASSERT_EQ(a.undo_head, 3);
    ASSERT_EQ(a.undo_tail, 4);
    a.add(7);
    LOG_MAGNUM_DEBUG << a.toString();
    ASSERT_EQ(a.buf[0], 5);
    ASSERT_EQ(a.buf[1], 6);
    ASSERT_EQ(a.buf[2], 7);
    ASSERT_EQ(a.main_head, 1);
    ASSERT_EQ(a.main_tail, 0);
    ASSERT_EQ(a.undo_head, 3);
    ASSERT_EQ(a.undo_tail, 5);
    a.undo();
    LOG_MAGNUM_DEBUG << a.toString();
    ASSERT_EQ(a.buf[0], 5);
    ASSERT_EQ(a.buf[1], 6);
    ASSERT_EQ(a.buf[2], 0);
    ASSERT_EQ(a.main_head, 0);
    ASSERT_EQ(a.main_tail, 2);
    ASSERT_EQ(a.undo_head, 3);
    ASSERT_EQ(a.undo_tail, 4);
    a.undo();
    LOG_MAGNUM_DEBUG << a.toString();
    ASSERT_EQ(a.buf[0], 5);
    ASSERT_EQ(a.buf[1], 0);
    ASSERT_EQ(a.buf[2], 0);
    ASSERT_EQ(a.main_head, 0);
    ASSERT_EQ(a.main_tail, 1);
    ASSERT_EQ(a.undo_head, 3);
    ASSERT_EQ(a.undo_tail, 3);
    a.undo();
    LOG_MAGNUM_DEBUG << a.toString();
    ASSERT_EQ(a.buf[0], 0);
    ASSERT_EQ(a.buf[1], 0);
    ASSERT_EQ(a.buf[2], 0);
    ASSERT_EQ(a.main_head, 0);
    ASSERT_EQ(a.main_tail, 0);
    ASSERT_EQ(a.undo_head, 3);
    ASSERT_EQ(a.undo_tail, 3);
}

TEST(CircularBuffer_Core, wrap_around_undo) {
    CircularBuffer a(3);
    a.add(5);
    a.add(6);
    a.add(7);
    LOG_MAGNUM_DEBUG << a.toString();
    a.add(8);
    LOG_MAGNUM_DEBUG << a.toString();
    ASSERT_EQ(a.buf[0], 8);
    ASSERT_EQ(a.buf[1], 6);
    ASSERT_EQ(a.buf[2], 7);
    ASSERT_EQ(a.main_head, 2);
    ASSERT_EQ(a.main_tail, 1);
    ASSERT_EQ(a.undo_head, 4);
    ASSERT_EQ(a.undo_tail, 3);
    a.undo();
    LOG_MAGNUM_DEBUG << a.toString();
    ASSERT_EQ(a.buf[0], 5);
    ASSERT_EQ(a.buf[1], 6);
    ASSERT_EQ(a.buf[2], 7);
    ASSERT_EQ(a.main_head, 1);
    ASSERT_EQ(a.main_tail, 0);
    ASSERT_EQ(a.undo_head, 3);
    ASSERT_EQ(a.undo_tail, 5);
}

TEST(CircularBuffer_Core, THIS_WILL_BREAK) {
    CircularBuffer x(50);

    LOG_MAGNUM_DEBUG << x.toString();

    for (int i = 0; i < 50; i++) {
        x.add(i);
    }

    LOG_MAGNUM_DEBUG << x.toString();

    for (int i = 0; i < 50; i++) {
        LOG_MAGNUM_DEBUG << "undoing add " << (49 - i) << std::endl;
        x.undo();
    }

    LOG_MAGNUM_DEBUG << x.toString();
}