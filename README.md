# Undo Redo Circular Buffer

Undo Redo Circular Buffer is a C++17 compatible library

with full support for Undo and Redo

and with customizable buffer sizes

```C++
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
```

# cloning

## as a non-submodule

```shell script
git clone --recursive https://github.com/mgood7123/Undo-Redo-CircularBuffer
```

## as a submodule

```shell script
git submodule add https://github.com/mgood7123/Undo-Redo-CircularBuffer path/where/Undo-Redo-CircularBuffer/should/reside/Undo-Redo-CircularBuffer
git submodule update --init --recursive
```

# building and testing

```shell script
make rebuild_test_debug
```