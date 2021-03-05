#include <JniHelpers.h>
#include <CircularBuffer.hpp>

#define makeVoidPtr(what) reinterpret_cast<void*>(what)
#define makejlong(what) reinterpret_cast<jlong>(what)

extern "C"
JNIEXPORT jlong JNICALL
Java_smallville7123_circularbuffer_CircularBuffer_createNativeInstance(JNIEnv *env, jobject thiz, jint size) {
    return makejlong(new CircularBuffer(size));
}

CircularBuffer * get(long ptr) {
    return reinterpret_cast<CircularBuffer*>(ptr);
}

extern "C"
JNIEXPORT jstring JNICALL
Java_smallville7123_circularbuffer_CircularBuffer_toString(JNIEnv *env, jobject thiz, jlong instance) {
    return JniHelpers::Strings::newString(env, get(instance)->toString());
}