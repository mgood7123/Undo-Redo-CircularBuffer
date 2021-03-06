cmake_minimum_required(VERSION 3.4.1)

project(CircularBuffer)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# https://doc.magnum.graphics/corrade/corrade-cmake.html#corrade-cmake-subproject

set(WITH_CONTAINERS ON CACHE BOOL "" FORCE)
set(WITH_PLUGINMANAGER OFF CACHE BOOL "" FORCE)
set(WITH_UTILITY ON CACHE BOOL "" FORCE)
set(WITH_TESTSUITE OFF CACHE BOOL "" FORCE)
set(WITH_RC OFF CACHE BOOL "" FORCE)
set(BUILD_STATIC OFF CACHE BOOL "" FORCE)

add_subdirectory(corrade EXCLUDE_FROM_ALL)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/corrade/modules/" ${CMAKE_MODULE_PATH})
find_package(Corrade REQUIRED Containers Utility)

include_directories(${CORRADE_INCLUDE_DIR})
include_directories(SPSCQueue/include)
include_directories(include)

if(NOT COMMAND testBuilder_build)
    add_subdirectory(testBuilder)
    testBuilder_set_current_working_directory_to_default_binary_directory()
endif()

testBuilder_add_source(UndoRedoCircularBuffer src/Log.cpp)
testBuilder_add_source(UndoRedoCircularBuffer src/JniHelpers.cpp)
testBuilder_add_source(UndoRedoCircularBuffer src/UndoRedoCircularBuffer.cpp)
testBuilder_add_source(UndoRedoCircularBuffer src/UndoRedoCircularBufferJNI.cpp)

testBuilder_add_library(UndoRedoCircularBuffer Corrade::Containers)
testBuilder_add_library(UndoRedoCircularBuffer Corrade::Utility)

testBuilder_build_shared_library(UndoRedoCircularBuffer)

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(googletest)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include")
endif()

# Now simply link against gtest or gtest_main as needed. Eg

add_subdirectory(tests)

set(CircularBuffer_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE)
