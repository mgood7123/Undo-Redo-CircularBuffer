package smallville7123.circularbuffer;

public class UndoRedoCircularBuffer {
    static {
        System.loadLibrary("UndoRedoCircularBuffer");
    }

    long instance;

    native long createNativeInstance1(long size);
    native long createNativeInstance2(long size, long undo_redo_size);
    native long createNativeInstance3(long size, long undo_size, long redo_size);
    native void add(long instance, long value);
    native long peek(long instance);
    native long remove(long instance);
    native void undo(long instance);
    native void redo(long instance);
    native String toString(long instance);

    public UndoRedoCircularBuffer(long size) {
        instance = createNativeInstance1(size);
    }

    UndoRedoCircularBuffer(long size, long undo_redo_size) {
        instance = createNativeInstance2(size, undo_redo_size);
    }

    UndoRedoCircularBuffer(long size, long undo_size, long redo_size) {
        instance = createNativeInstance3(size, undo_size, redo_size);
    }

    void add(long value) {
        add(instance, value);
    }

    long peek() {
        return peek(instance);
    }

    long remove() {
        return remove(instance);
    }

    void undo() {
        undo(instance);
    }

    void redo() {
        redo(instance);
    }

    @Override
    public String toString() {
        return toString(instance);
    }
}