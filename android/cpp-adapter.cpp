#include <jni.h>
#include "react-native-msgpack.hpp"

extern "C" JNIEXPORT void JNICALL
Java_com_msgpack_MsgpackModule_nativeInstall(JNIEnv *env, jobject thiz, jlong jsi)
{
  auto runtime = reinterpret_cast<jsi::Runtime *>(jsi);
  install(*runtime);
}
