package smallville7123.circularbuffer;

public class UndoRedoCircularBuffer {
    static {
        System.loadLibrary("UndoRedoCircularBuffer");
    }

    long instance;

    native long createNativeInstance1(int size);
    native long createNativeInstance2(int size, int undo_redo_size);
    native long createNativeInstance3(int size, int undo_size, int redo_size);
    native String toString(long instance);

    public UndoRedoCircularBuffer(int size) {
        instance = createNativeInstance1(size);
    }

    UndoRedoCircularBuffer(int size, int undo_redo_size) {
        instance = createNativeInstance2(size, undo_redo_size);
    }

    UndoRedoCircularBuffer(int size, int undo_size, int redo_size) {
        instance = createNativeInstance3(size, undo_size, redo_size);
    }

    @Override
    public String toString() {
        return toString(instance);
    }
}