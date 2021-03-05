#include <JniHelpers.h>
#include <UndoRedoCircularBuffer.hpp>

#define makeVoidPtr(what) reinterpret_cast<void*>(what)
#define makejlong(what) reinterpret_cast<jlong>(what)

extern "C"
JNIEXPORT jlong JNICALL
Java_smallville7123_circularbuffer_UndoRedoCircularBuffer_createNativeInstance1(JNIEnv *env, jobject thiz, jint size) {
    return makejlong(new UndoRedoCircularBuffer(size));
}

extern "C"
JNIEXPORT jlong JNICALL
Java_smallville7123_circularbuffer_UndoRedoCircularBuffer_createNativeInstance2(JNIEnv *env, jobject thiz, jint size, jint undo_redo_size) {
    return makejlong(new UndoRedoCircularBuffer(size, undo_redo_size));
}

extern "C"
JNIEXPORT jlong JNICALL
Java_smallville7123_circularbuffer_UndoRedoCircularBuffer_createNativeInstance3(JNIEnv *env, jobject thiz, jint size, jint undo_size, jint redo_size) {
    return makejlong(new UndoRedoCircularBuffer(size, undo_size, redo_size));
}

UndoRedoCircularBuffer * get(long ptr) {
    return reinterpret_cast<UndoRedoCircularBuffer*>(ptr);
}

extern "C"
JNIEXPORT jstring JNICALL
Java_smallville7123_circularbuffer_UndoRedoCircularBuffer_toString(JNIEnv *env, jobject thiz, jlong instance) {
    return JniHelpers::Strings::newString(env, get(instance)->toString());
}