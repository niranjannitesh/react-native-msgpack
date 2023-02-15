#include <jni.h>
#include "react-native-msgpack.h"

extern "C"
JNIEXPORT jint JNICALL
Java_com_msgpack_MsgpackModule_nativeMultiply(JNIEnv *env, jclass type, jdouble a, jdouble b) {
    return msgpack::multiply(a, b);
}
