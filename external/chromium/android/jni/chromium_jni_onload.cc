#include <jni.h>
#include <JNIHelp.h>
#include "base/logging.h"
#include "net/socket/ssl_client_socket_openssl.h"

bool setOpenSslContext(JNIEnv *env, jobject obj, jboolean cac);
bool registerCACEngine(JNIEnv *env, jobject obj);
void deregisterCACEngine(JNIEnv *env, jobject obj);
int getCACEngineState(JNIEnv *env, jobject obj);

static JavaVM* gpVm = NULL;
static JNINativeMethod methods[] = {
  {"nativeSetOpenSslContext", "(Z)Z", (void*)&setOpenSslContext},
  {"nativeRegisterCACEngine", "()Z", (void*)&registerCACEngine},
  {"nativeDeregisterCACEngine", "()V", (void*)&deregisterCACEngine},
  {"nativeGetCACEngineState", "()I", (void*)&getCACEngineState},
};

jint JNI_OnLoad(JavaVM* vm, void* reserved) {
  gpVm = vm;
  JNIEnv* pEnv = NULL;
  gpVm->AttachCurrentThread(&pEnv,NULL);
  jniRegisterNativeMethods(pEnv, "android/webkit/JniUtil", methods, NELEM(methods));
  LOG(INFO) << "JNI_OnLoad";
  return JNI_VERSION_1_4;
}

bool setOpenSslContext(JNIEnv *env, jobject obj, jboolean cac) {
  return net::SSLClientSocketOpenSSL::setOpenSslContext(cac);
}
bool registerCACEngine(JNIEnv *env, jobject obj) {
  return net::SSLClientSocketOpenSSL::registerCACEngine();
}
void deregisterCACEngine(JNIEnv *env, jobject obj) {
  net::SSLClientSocketOpenSSL::deregisterCACEngine();
}
int getCACEngineState(JNIEnv *env, jobject obj) {
  return net::SSLClientSocketOpenSSL::getCACEngineState();
}