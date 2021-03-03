#include <CircularBuffer.hpp>
#include <Log.hpp>

void CircularBuffer::println(std::string x) {
    LOG_MAGNUM_INFO << x;
}

void CircularBuffer::test() {
    CircularBuffer x = CircularBuffer(2, 10);
    println(x.toString());
    x.add(1);
    println(x.toString());
    x.add(2);
    println(x.toString());
    x.add(3);
    println(x.toString());
    x.add(4);
    println(x.toString());
    x.add(5);
    println(x.toString());
    x.add(6);
    println(x.toString());
    x.undo();
    println(x.toString());
    x.undo();
    println(x.toString());
    x.undo();
    println(x.toString());
    x.undo();
    println(x.toString());
    x.undo();
    println(x.toString());
}

CircularBuffer::CircularBuffer(int size) : CircularBuffer(size, size, size) {}

CircularBuffer::CircularBuffer(int size, int undo_redo_size) : CircularBuffer(size, undo_redo_size, undo_redo_size) {}

CircularBuffer::CircularBuffer(int size, int undo_size, int redo_size) {
    CBUF_SIZE = size + 1;
    CBUF_SIZE_UNDO = undo_size + 1;
    CBUF_SIZE_REDO = redo_size + 1;
    buf = new int[CBUF_SIZE];
    memset(buf, 0, CBUF_SIZE * SIZE_OF_INT);
    buf_UNDO = new int[CBUF_SIZE_UNDO];
    memset(buf_UNDO, 0, CBUF_SIZE_UNDO * SIZE_OF_INT);
    buf_REDO = new int[CBUF_SIZE_REDO];
    memset(buf_REDO, 0, CBUF_SIZE_REDO * SIZE_OF_INT);
    head = 0;
    tail = 0;
    head_UNDO = 0;
    tail_UNDO = 0;
    head_REDO = 0;
    tail_REDO = 0;
    println(
            "CircularBuffer initialized with capacity of " + std::to_string(size) + " plus 1 for the tail marker\n" +
            "CircularBuffer initialized undo with capacity of " + std::to_string(undo_size) + " plus 1 for the tail marker\n" +
            "CircularBuffer initialized redo with capacity of " + std::to_string(redo_size) + " plus 1 for the tail marker"
    );
}

CircularBuffer::~CircularBuffer() {
    delete[] buf;
    delete[] buf_UNDO;
    delete[] buf_REDO;
}

int CircularBuffer::size(int head, int tail, int CBUF_SIZE) {
    return head <= tail
           ? tail - head
           : CBUF_SIZE - (head - tail);
}

int CircularBuffer::size() {
    return size(head, tail, CBUF_SIZE);
}

void CircularBuffer::add(int n) {
    println("adding " + std::to_string(n));

    if (head != tail) {
        if (tail == 0) {
            buf_UNDO[tail_UNDO++] = buf[CBUF_SIZE-1];
        } else {
            buf_UNDO[tail_UNDO++] = buf[tail-1];
        }
        tail_UNDO %= CBUF_SIZE_UNDO;
        buf_UNDO[tail_UNDO] = 0; // for illustration
        if (tail_UNDO == head_UNDO) head_UNDO++;
        // if (head == 1 && tail == 0) {
        //     head_UNDO += 3;
        //     head_UNDO %= CBUF_SIZE_UNDO;
        //     println("head_UNDO " + head_UNDO + " module " + CBUF_SIZE_UNDO + " = " + head_UNDO);
        // }
    }

    buf[tail++] = n;
    tail %= CBUF_SIZE;
    buf[tail] = 0; // for illustration
    if (tail == head) {
        head++;
        head %= CBUF_SIZE;
    }
}

void CircularBuffer::undo() {
    println("undo");
    if (head == tail) {
        return;
    }
    if (tail == head - 1) {
        head--;
        buf[head] = buf_UNDO[head_UNDO];
        buf_UNDO[head_UNDO] = 0;
        head_UNDO++;
        head_UNDO %= CBUF_SIZE_UNDO;
    } else {
        if (tail_UNDO == 0) tail_UNDO = CBUF_SIZE_UNDO-1;
        else {
            tail_UNDO--;
            buf_UNDO[tail_UNDO] = 0;
            if (tail_UNDO != 0 && head_UNDO == tail_UNDO) head_UNDO--;
        }
        // what should we do here?
        //
        // head--;
        // buf[head] = buf_UNDO[head_UNDO];
        // buf_UNDO[head_UNDO] = 0;
        // head_UNDO++;
        // head_UNDO %= CBUF_SIZE_UNDO;
    }
    if (tail == 0) tail = CBUF_SIZE-1;
    else tail--;
    int n = buf[tail];
    buf[tail] = 0; // for illustration
}

int CircularBuffer::peek() {
    if (head == tail) {
        println("queue empty, oh noes\n");
        return 0;
    }

    int n = buf[head];
    buf[head] = 0; // for illustration
    println("peek " + std::to_string(n));
    return n;
}

int CircularBuffer::remove() {
    if (head == tail) {
        println("queue empty, oh noes\n");
        return 0;
    }

    int n = buf[head];
    buf[head] = 0; // for illustration
    head++;
    head %= CBUF_SIZE;
    println("removed " + std::to_string(n));
    return n;
}

template<typename... Args>
std::string CircularBuffer::format(const std::string &format, Args... args) {
    int size = snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
    if( size <= 0 ) {
        throw std::runtime_error( "Error during formatting." );
    }
    std::unique_ptr<char[]> buf( new char[ size ] );
    snprintf( buf.get(), size, format.c_str(), args ... );
    return std::string( buf.get(), buf.get() + size - 1 );
}

std::string CircularBuffer::toString(int CBUF_SIZE, int head, int tail, int *buffer) {
    std::string buf = "";
    for (int i = 0; i < CBUF_SIZE; i++) {
        if (head == i) buf += format("%4c", 'h');
        else if (tail == i) buf += format("%4c", 't');
        else buf += format("%4c", ' ');
    }
    buf += "\n";

    for (int i = 0; i < CBUF_SIZE; i++) {
        buf += format("%4d", buffer[i]);
    }

    buf += format("   (%d)", size(head, tail, CBUF_SIZE));
    buf += "\n";
    return buf;
}

std::string CircularBuffer::toString() {
    std::string string = "";
    string += "MAIN:\n" + toString(CBUF_SIZE, head, tail, buf) + "\n";
    string += "UNDO:\n" + toString(CBUF_SIZE_UNDO, head_UNDO, tail_UNDO, buf_UNDO) + "\n";
    string += "REDO:\n" + toString(CBUF_SIZE_REDO, head_REDO, tail_REDO, buf_REDO) + "\n";
    return string;
}
