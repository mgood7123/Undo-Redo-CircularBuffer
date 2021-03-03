//
// Created by smallville7123 on 9/08/20.
//

#include <gtest/gtest.h>

#include <CircularBuffer.hpp>

TEST(CircularBuffer_Core, initialization_no_data_checking) {
    CircularBuffer a(1);
}

TEST(CircularBuffer_Core, initialization_data_checking) {
    CircularBuffer a(1);
    ASSERT_NE(a.buf, nullptr);
    ASSERT_EQ(a.head, 0);
    ASSERT_EQ(a.tail, 0);
    ASSERT_NE(a.buf_UNDO, nullptr);
    ASSERT_EQ(a.head_UNDO, 0);
    ASSERT_EQ(a.tail_UNDO, 0);
    ASSERT_NE(a.buf_REDO, nullptr);
    ASSERT_EQ(a.head_REDO, 0);
    ASSERT_EQ(a.tail_UNDO, 0);
    ASSERT_EQ(a.size(), 0);
    ASSERT_EQ(a.CBUF_SIZE, 2);
    ASSERT_EQ(a.buf[0], 0);
    ASSERT_EQ(a.buf[1], 0);
    LOG_MAGNUM_DEBUG << a.toString();
}

TEST(CircularBuffer_Core, add) {
    CircularBuffer a(1);
    LOG_MAGNUM_DEBUG << a.toString();
    a.add(5);
    LOG_MAGNUM_DEBUG << a.toString();
    LOG_MAGNUM_DEBUG_FUNCTION(a.head);
    LOG_MAGNUM_DEBUG_FUNCTION(a.tail);
    ASSERT_EQ(a.head, 0);
    ASSERT_EQ(a.tail, 1);
}

TEST(CircularBuffer_Core, add_then_undo) {
    CircularBuffer a(1);
    LOG_MAGNUM_DEBUG << a.toString();
    a.add(5);
    LOG_MAGNUM_DEBUG << a.toString();
    LOG_MAGNUM_DEBUG_FUNCTION(a.head);
    LOG_MAGNUM_DEBUG_FUNCTION(a.tail);
    a.undo();
    LOG_MAGNUM_DEBUG << a.toString();
    LOG_MAGNUM_DEBUG_FUNCTION(a.head);
    LOG_MAGNUM_DEBUG_FUNCTION(a.tail);
    ASSERT_EQ(a.head, 0);
    ASSERT_EQ(a.tail, 0);
}