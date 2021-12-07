#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<jni.h>
#include<android/log.h>
#include <android_runtime/AndroidRuntime.h>
#include <JNIHelp.h>

#include "usb-unix.h"

using namespace android;

#define LOG_TAG "GenericPrintService"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
typedef void (*update_state)(int,unsigned int);
typedef void (*create_java_thread)(const char* name, void (*start)(void *), void* arg);

static jobject mCallbacksObj = NULL;
static jobject mSearchCallbacksObj = NULL;
static JNIEnv* Env = NULL;
static jmethodID method_reportJobState;


extern "C" int ipp_main(int  argc,char *argv[], update_state ptr, create_java_thread thread_fun);
extern "C" int imagetops_main(int  argc,char *argv[]);
extern "C" int usb_main(int  argc,char *argv[]); 
extern "C" Usb_Device_Info * get_devices();
extern "C" Usb_Device_Info * getDevFile_devices();
extern "C" int socket_main(int  argc,char *argv[],update_state ptr);
extern "C" int usbDevFile_main(int  argc,char *argv[],update_state ptr); 


extern "C" void update_JobState(int type, unsigned int value)
{
	jlong value_l = (jlong)(unsigned long long)value;
	
	LOGI("update_JobState type = %d, value_l = %ld",type,value_l);
	JNIEnv *env = AndroidRuntime::getJNIEnv();
	env->CallVoidMethod(mCallbacksObj, method_reportJobState, type, value_l);
}


extern "C" void createJavaThread(const char* name, void (*start)(void *), void* arg)
{
	AndroidRuntime::createJavaThread(name, start, arg);
}



jint Java_com_siso_app_genericprintservice_GPrintPrintingManager_cancelJob(JNIEnv* env, jobject thiz,jstring cancelString){
	return 1;
}

jint Java_com_siso_app_genericprintservice_GPrintPrintingManager_InvokeImageToPs(JNIEnv* env, jobject thiz,jobjectArray stringArray){

	char *argv[100];
	int stringCount = env->GetArrayLength(stringArray);
	int i,count,lpout=0;

    for (i=0; i<stringCount; i++) {
        jstring string = (jstring) env->GetObjectArrayElement(stringArray, i);
		const char *arg = env->GetStringUTFChars(string, 0);
		
		argv[i] = const_cast<char *> (arg);
	}

	lpout = imagetops_main(stringCount,argv);
	return lpout;


}

jint Java_com_siso_app_genericprintservice_GPrintPrintingManager_InvokeIppBackend(JNIEnv* env, jobject thiz,jobjectArray stringArray){

	Env = env;
	if (!mCallbacksObj)
        mCallbacksObj = env->NewGlobalRef(thiz);
	
	jclass clazz = env->FindClass("com/siso/app/genericprintservice/GPrintPrintingManager");

	jclass glo_clazz = reinterpret_cast<jclass> (env->NewGlobalRef(clazz));
	
	method_reportJobState = env->GetMethodID(glo_clazz, "reportJobState", "(IJ)V");
	 
	char *argv[100];
	int stringCount = env->GetArrayLength(stringArray);
	int i,count,lpout=0;

    for (i=0; i<stringCount; i++) {
        jstring string = (jstring) env->GetObjectArrayElement(stringArray, i);
        const char *arg = env->GetStringUTFChars(string, 0);
		argv[i] = const_cast<char *> (arg);
		LOGI("JNI InvokeIppBackend argv = %s",argv[i]);
    }
	
	lpout = ipp_main(stringCount,argv, &update_JobState, &createJavaThread);
	return lpout;


}


jint Java_com_siso_app_genericprintservice_GPrintPrintingManager_InvokeSocketBackend(JNIEnv* env, jobject thiz,jobjectArray stringArray){

	Env = env;
	if (!mCallbacksObj)
        mCallbacksObj = env->NewGlobalRef(thiz);
	
	jclass clazz = env->FindClass("com/siso/app/genericprintservice/GPrintPrintingManager");

	jclass glo_clazz = reinterpret_cast<jclass> (env->NewGlobalRef(clazz));
	
	method_reportJobState = env->GetMethodID(glo_clazz, "reportJobState", "(IJ)V");
	char *argv[100];
	int stringCount = env->GetArrayLength(stringArray);
	int i,count,lpout=0;

    for (i=0; i<stringCount; i++) {
        jstring string = (jstring) env->GetObjectArrayElement(stringArray, i);
        const char *arg = env->GetStringUTFChars(string, 0);
		argv[i] = const_cast<char *> (arg);
		LOGI("JNI InvokeSocketBackend argv = %s",argv[i]);
    }

	lpout = socket_main(stringCount,argv, &update_JobState);
	return lpout;


}

jobjectArray Java_com_siso_app_genericprintservice_GPrintPrintingManager_GetDevFileUsbDevice(JNIEnv *env, jobject thiz)
{
	 Usb_Device_Info *device_info =  getDevFile_devices();
	 jclass stringClazz = env->FindClass("java/lang/String");
	 jobjectArray stringArray = env->NewObjectArray(3, stringClazz, NULL);

	 if(device_info->device_uri != NULL)
	{
		 jstring uri = env->NewStringUTF(device_info->device_uri);
		 env->SetObjectArrayElement(stringArray, 0, uri);
		 env->DeleteLocalRef(uri);
	} 
     
	 if(device_info->make_model != NULL)
	{
		 jstring model = env->NewStringUTF(device_info->make_model);
		 env->SetObjectArrayElement(stringArray, 1, model);
		 env->DeleteLocalRef(model);
	 } 

	if(device_info->device_id != NULL)
	 {
		 jstring device_id = env->NewStringUTF(device_info->device_id);
		 env->SetObjectArrayElement(stringArray, 2, device_id);
		 env->DeleteLocalRef(device_id);
	 } 
	 
	 return stringArray;
}

jint Java_com_siso_app_genericprintservice_GPrintPrintingManager_InvokeDevFileUsbBackend(JNIEnv* env, jobject thiz,jobjectArray stringArray){
	Env = env;
	if (!mCallbacksObj)
        mCallbacksObj = env->NewGlobalRef(thiz);
	
	jclass clazz = env->FindClass("com/siso/app/genericprintservice/GPrintPrintingManager");

	jclass glo_clazz = reinterpret_cast<jclass> (env->NewGlobalRef(clazz));
	
	method_reportJobState = env->GetMethodID(glo_clazz, "reportJobState", "(IJ)V");
	
	char *argv[100];
	int stringCount = env->GetArrayLength(stringArray);
	int i,count,lpout=0;

	LOGI("The value of String Count is = %d",stringCount);

	for (i=0; i<stringCount; i++) {
		jstring string = (jstring) env->GetObjectArrayElement(stringArray, i);
		const char *arg = env->GetStringUTFChars(string, 0);
		argv[i] = const_cast<char *> (arg);
		LOGI("Calculated argv[]");
		LOGI("The value of argv = %s",argv[i]);
	}
    lpout = usbDevFile_main(stringCount,argv, &update_JobState);
	return lpout;

}

jobjectArray Java_com_siso_app_genericprintservice_GPrintPrintingManager_GetUsbDeviceList(JNIEnv *env, jobject thiz)
{
	LOGI("GetUsbDeviceList");	 
	 Usb_Device_Info *device_info =  get_devices();
	 jclass stringClazz = env->FindClass("java/lang/String");
	 jobjectArray stringArray = env->NewObjectArray(3, stringClazz, NULL);

	 if(device_info->device_uri != NULL)
	{
		 jstring uri = env->NewStringUTF(device_info->device_uri);
		 env->SetObjectArrayElement(stringArray, 0, uri);
		 env->DeleteLocalRef(uri);
	} 
     
	 if(device_info->make_model != NULL)
	{
		 jstring model = env->NewStringUTF(device_info->make_model);
		 env->SetObjectArrayElement(stringArray, 1, model);
		 env->DeleteLocalRef(model);
	 } 

	if(device_info->device_id != NULL)
	 {
		 jstring device_id = env->NewStringUTF(device_info->device_id);
		 env->SetObjectArrayElement(stringArray, 2, device_id);
		 env->DeleteLocalRef(device_id);
	 	} 
	 
	 return stringArray;
}

jint Java_com_siso_app_genericprintservice_GPrintPrintingManager_InvokeUsbBackend(JNIEnv* env, jobject thiz,jobjectArray stringArray){

	char *argv[100];
	int stringCount = env->GetArrayLength(stringArray);
	int i,count,lpout=0;

	for (i=0; i<stringCount; i++) {
		jstring string = (jstring) env->GetObjectArrayElement(stringArray, i);
		const char *arg = env->GetStringUTFChars(string, 0);
		argv[i] = const_cast<char *> (arg);
		LOGI("JNI InvokeUsbBackend argv = %s",argv[i]);
	}
    
	
	lpout = usb_main(stringCount,argv);
	return lpout;


}



//{"cancelJob", "(Ljava/lang/String;)I", (void *)Java_com_siso_app_genericprintservice_GPrintPrintingManager_cancelJob},


static JNINativeMethod sMethods[] = {
     /* name, signature, funcPtr */
	
	{"InvokeImageToPs", "([Ljava/lang/String;)I", (void *)Java_com_siso_app_genericprintservice_GPrintPrintingManager_InvokeImageToPs},
	{"InvokeIppBackend", "([Ljava/lang/String;)I", (void *)Java_com_siso_app_genericprintservice_GPrintPrintingManager_InvokeIppBackend},
	{"GetUsbDeviceList", "()[Ljava/lang/String;",(void *) Java_com_siso_app_genericprintservice_GPrintPrintingManager_GetUsbDeviceList},
	{"InvokeUsbBackend", "([Ljava/lang/String;)I", (void *)Java_com_siso_app_genericprintservice_GPrintPrintingManager_InvokeUsbBackend},
	{"GetDevFileUsbDevice", "()[Ljava/lang/String;",(void *) Java_com_siso_app_genericprintservice_GPrintPrintingManager_GetDevFileUsbDevice},
	{"InvokeDevFileUsbBackend", "([Ljava/lang/String;)I", (void *)Java_com_siso_app_genericprintservice_GPrintPrintingManager_InvokeDevFileUsbBackend},
	{"InvokeSocketBackend", "([Ljava/lang/String;)I", (void *)Java_com_siso_app_genericprintservice_GPrintPrintingManager_InvokeSocketBackend}
		
};




int register_android_jni_cups(JNIEnv* env)  
{	
	jniRegisterNativeMethods(env, "com/siso/app/genericprintservice/GPrintPrintingManager", sMethods, NELEM(sMethods));
	
	
    return 0;
}

extern "C" jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    JNIEnv* env = NULL;
    jint result = -1;

    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        LOGI("JNI ERROR GetEnv failed!");
        return result;
    }
    register_android_jni_cups(env);

    return JNI_VERSION_1_4;
}


