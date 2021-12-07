/*
 * Copyright 2012, The Android Open Source Project //4.2 Merge 
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

// The functions in this file are used to configure the mock GeolocationService
// for the LayoutTests.

#include "config.h"

//// 4.2 Merge BEGIN <<
#include "WebViewCore.h"

#include <GeolocationError.h>
#include <GeolocationPosition.h>
#include "JNIHelp.h"
#include "ScopedLocalRef.h"
// 4.2 Merge END >>
#include <wtf/CurrentTime.h>

using namespace WebCore;

namespace android {

static const char* javaMockGeolocationClass = "android/webkit/MockGeolocation";

//// 4.2 Merge BEGIN <<
WebViewCore* getWebViewCore(JNIEnv* env, jobject webViewCore)
{
    ScopedLocalRef<jclass> webViewCoreClass(env, env->FindClass("android/webkit/WebViewCore"));
    jfieldID nativeClassField = env->GetFieldID(webViewCoreClass.get(), "mNativeClass", "I");
    return reinterpret_cast<WebViewCore*>(env->GetIntField(webViewCore, nativeClassField));
}

static void setUseMock(JNIEnv* env, jobject, jobject webViewCore)
{
     // 4.2 Merge : commented for compilation error
    getWebViewCore(env, webViewCore)->geolocationManager()->setUseMock();
}

static void setPosition(JNIEnv* env, jobject, jobject webViewCore, double latitude, double longitude, double accuracy)
{
     // 4 .2 Merge : commented for compilation error
    getWebViewCore(env, webViewCore)->geolocationManager()->setMockPosition(GeolocationPosition::create(WTF::currentTime(),
                                                                                                        latitude,
                                                                                                        longitude,
                                                                                                        accuracy,
                                                                                                        false, 0.0,  // altitude,
                                                                                                        false, 0.0,  // altitudeAccuracy,
                                                                                                        false, 0.0,  // heading
                                                                                                        false, 0.0));  // speed
}

static void setError(JNIEnv* env, jobject, jobject webViewCore, int code, jstring message)
{
     // 4 .2 Merge : commented for compilation error
    GeolocationError::ErrorCode codeEnum = static_cast<GeolocationError::ErrorCode>(code);
    getWebViewCore(env, webViewCore)->geolocationManager()->setMockError(GeolocationError::create(codeEnum, jstringToWtfString(env, message)));
}

static void setPermission(JNIEnv* env, jobject, jobject webViewCore, bool allow)
{
    //  4 .2 Merge : commented for compilation error
    getWebViewCore(env, webViewCore)->geolocationManager()->setMockPermission(allow);
}
// 4.2 Merge END >>
static JNINativeMethod gMockGeolocationMethods[] = {
    { "nativeSetUseMock", "(Landroid/webkit/WebViewCore;)V", (void*) setUseMock },//4   .2 Merge
    { "nativeSetPosition", "(Landroid/webkit/WebViewCore;DDD)V", (void*) setPosition }, //4.2 Merge
    { "nativeSetError", "(Landroid/webkit/WebViewCore;ILjava/lang/String;)V", (void*) setError },//4.2 Merge
    { "nativeSetPermission", "(Landroid/webkit/WebViewCore;Z)V", (void*) setPermission },//4.2 Merge
};

int registerMockGeolocation(JNIEnv* env)
{
#ifndef NDEBUG
    jclass mockGeolocation = env->FindClass(javaMockGeolocationClass);
    ALOG_ASSERT(mockGeolocation, "Unable to find class");
    env->DeleteLocalRef(mockGeolocation);
#endif

    return jniRegisterNativeMethods(env, javaMockGeolocationClass, gMockGeolocationMethods, NELEM(gMockGeolocationMethods));
}

}
