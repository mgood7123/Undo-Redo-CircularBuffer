#include <JniHelpers.h>
#include <CircularBuffer.hpp>

#define makeVoidPtr(what) reinterpret_cast<void*>(what)
#define makejlong(what) reinterpret_cast<jlong>(what)

extern "C"
JNIEXPORT jlong JNICALL
Java_smallville7123_circularbuffer_CircularBuffer_createNativeInstance1(JNIEnv *env, jobject thiz, jint size) {
    return makejlong(new CircularBuffer(size));
}

extern "C"
JNIEXPORT jlong JNICALL
Java_smallville7123_circularbuffer_CircularBuffer_createNativeInstance2(JNIEnv *env, jobject thiz, jint size, jint undo_redo_size) {
    return makejlong(new CircularBuffer(size, undo_redo_size));
}

extern "C"
JNIEXPORT jlong JNICALL
Java_smallville7123_circularbuffer_CircularBuffer_createNativeInstance3(JNIEnv *env, jobject thiz, jint size, jint undo_size, jint redo_size) {
    return makejlong(new CircularBuffer(size, undo_size, redo_size));
}

CircularBuffer * get(long ptr) {
    return reinterpret_cast<CircularBuffer*>(ptr);
}

extern "C"
JNIEXPORT jstring JNICALL
Java_smallville7123_circularbuffer_CircularBuffer_toString(JNIEnv *env, jobject thiz, jlong instance) {
    return JniHelpers::Strings::newString(env, get(instance)->toString());
}