#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<jni.h>
#include<android/log.h>
#include <android_runtime/AndroidRuntime.h>
#include <JNIHelp.h>

using namespace android;

#define LOG_TAG "GenericPrintService"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)

extern "C" int gs_main(int argc, char *argv[]);
		
jint Java_com_siso_app_genericprintservice_GPrintPsToPcl_InvokeGhostScript(JNIEnv* env, jobject thiz,jobjectArray stringArray){

	char *argv[100];
	int stringCount = env->GetArrayLength(stringArray);
	int i,count,lpout=0;

    for (i=0; i<stringCount; i++) {
        jstring string = (jstring) env->GetObjectArrayElement(stringArray, i);
        const char *arg = env->GetStringUTFChars(string, 0);
		argv[i] = const_cast<char *> (arg);
		LOGI("JNI InvokeGhostScript argv = %s",argv[i]);
    }

	lpout = gs_main(stringCount,argv);
	return lpout;


}




static JNINativeMethod gsMethods[] = {
     /* name, signature, funcPtr */
	
	{"InvokeGhostScript", "([Ljava/lang/String;)I", (void *)Java_com_siso_app_genericprintservice_GPrintPsToPcl_InvokeGhostScript}
		
};


int register_android_jni_cups(JNIEnv* env)  
{	
	jniRegisterNativeMethods(env, "com/siso/app/genericprintservice/GPrintPsToPcl", gsMethods, NELEM(gsMethods));
	
	
    return 0;
}

extern "C" jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    JNIEnv* env = NULL;
    jint result = -1;

    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        LOGI("Error : GetEnv failed!");
        return result;
    }
    register_android_jni_cups(env);

    return JNI_VERSION_1_4;
}
