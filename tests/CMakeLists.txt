cmake_minimum_required(VERSION 3.4.1)

testBuilder_add_source(CircularBuffer_Tests CircularBuffer_Tests.cpp)
testBuilder_add_library(CircularBuffer_Tests gtest_main)
testBuilder_add_library(CircularBuffer_Tests UndoRedoCircularBuffer)
testBuilder_build(CircularBuffer_Tests EXECUTABLES)