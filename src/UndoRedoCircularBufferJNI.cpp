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
JNIEXPORT jboolean JNICALL
Java_smallville7123_circularbuffer_UndoRedoCircularBuffer_empty(JNIEnv *env, jobject thiz, jlong instance) {
    return get(instance)->empty();
}

extern "C"
JNIEXPORT jlong JNICALL
Java_smallville7123_circularbuffer_UndoRedoCircularBuffer_front(JNIEnv *env, jobject thiz, jlong instance) {
    return get(instance)->front();
}

extern "C"
JNIEXPORT jlong JNICALL
Java_smallville7123_circularbuffer_UndoRedoCircularBuffer_back(JNIEnv *env, jobject thiz, jlong instance) {
    return get(instance)->back();
}

extern "C"
JNIEXPORT void JNICALL
Java_smallville7123_circularbuffer_UndoRedoCircularBuffer_push_front(JNIEnv *env, jobject thiz, jlong instance, jlong value) {
    get(instance)->push_front(value);
}

extern "C"
JNIEXPORT void JNICALL
Java_smallville7123_circularbuffer_UndoRedoCircularBuffer_push_back(JNIEnv *env, jobject thiz, jlong instance, jlong value) {
    get(instance)->push_back(value);
}

extern "C"
JNIEXPORT jlong JNICALL
Java_smallville7123_circularbuffer_UndoRedoCircularBuffer_pop_front(JNIEnv *env, jobject thiz, jlong instance) {
    return get(instance)->pop_front();
}

extern "C"
JNIEXPORT jlong JNICALL
Java_smallville7123_circularbuffer_UndoRedoCircularBuffer_pop_back(JNIEnv *env, jobject thiz, jlong instance) {
    return get(instance)->pop_back();
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