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

    public UndoRedoCircularBuffer(long size, long undo_redo_size) {
        instance = createNativeInstance2(size, undo_redo_size);
    }

    public UndoRedoCircularBuffer(long size, long undo_size, long redo_size) {
        instance = createNativeInstance3(size, undo_size, redo_size);
    }

    public void add(long value) {
        add(instance, value);
    }

    public long peek() {
        return peek(instance);
    }

    public long remove() {
        return remove(instance);
    }

    public void undo() {
        undo(instance);
    }

    public void redo() {
        redo(instance);
    }

    @Override
    public String toString() {
        return toString(instance);
    }
}