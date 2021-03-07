#include <JniHelpers.h>
#include <UndoRedoCircularBuffer.hpp>

#define makeVoidPtr(what) reinterpret_cast<void*>(what)
#define makejlong(what) reinterpret_cast<jlong>(what)

extern "C"
JNIEXPORT jlong JNICALL
Java_smallville7123_circularbuffer_UndoRedoCircularBuffer_createNativeInstance1(JNIEnv *env, jobject thiz, jlong size) {
    return makejlong(new UndoRedoCircularBuffer(size));
}

extern "C"
JNIEXPORT jlong JNICALL
Java_smallville7123_circularbuffer_UndoRedoCircularBuffer_createNativeInstance2(JNIEnv *env, jobject thiz, jlong size, jlong undo_redo_size) {
    return makejlong(new UndoRedoCircularBuffer(size, undo_redo_size));
}

extern "C"
JNIEXPORT jlong JNICALL
Java_smallville7123_circularbuffer_UndoRedoCircularBuffer_createNativeInstance3(JNIEnv *env, jobject thiz, jlong size, jlong undo_size, jlong redo_size) {
    return makejlong(new UndoRedoCircularBuffer(size, undo_size, redo_size));
}

UndoRedoCircularBuffer * get(long ptr) {
    return reinterpret_cast<UndoRedoCircularBuffer*>(ptr);
}

extern "C"
JNIEXPORT jlong JNICALL
Java_smallville7123_circularbuffer_UndoRedoCircularBuffer_size(JNIEnv *env, jobject thiz, jlong instance) {
    return get(instance)->size();
}

extern "C"
JNIEXPORT void JNICALL
Java_smallville7123_circularbuffer_UndoRedoCircularBuffer_add(JNIEnv *env, jobject thiz, jlong instance, jlong value) {
    get(instance)->add(value);
}

extern "C"
JNIEXPORT jlong JNICALL
Java_smallville7123_circularbuffer_UndoRedoCircularBuffer_peek(JNIEnv *env, jobject thiz, jlong instance) {
    return get(instance)->peek();
}

extern "C"
JNIEXPORT jlong JNICALL
Java_smallville7123_circularbuffer_UndoRedoCircularBuffer_remove(JNIEnv *env, jobject thiz, jlong instance) {
    return get(instance)->remove();
}

extern "C"
JNIEXPORT void JNICALL
Java_smallville7123_circularbuffer_UndoRedoCircularBuffer_undo(JNIEnv *env, jobject thiz, jlong instance) {
    get(instance)->undo();
}

extern "C"
JNIEXPORT void JNICALL
Java_smallville7123_circularbuffer_UndoRedoCircularBuffer_redo(JNIEnv *env, jobject thiz, jlong instance) {
    get(instance)->redo();
}

extern "C"
JNIEXPORT jstring JNICALL
Java_smallville7123_circularbuffer_UndoRedoCircularBuffer_toString(JNIEnv *env, jobject thiz, jlong instance) {
    return JniHelpers::Strings::newString(env, get(instance)->toString());
}