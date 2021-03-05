package smallville7123.circularbuffer;

public class CircularBuffer {
    static {
        System.loadLibrary("CircularBuffer");
    }

    long instance;

    native long createNativeInstance(int size);
    native String toString(long instance);

    public CircularBuffer(int size) {
        instance = createNativeInstance(size);
    }

    @Override
    public String toString() {
        return toString(instance);
    }
}