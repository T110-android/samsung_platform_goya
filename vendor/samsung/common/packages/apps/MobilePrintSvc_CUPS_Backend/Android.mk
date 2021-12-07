
LOCAL_PATH := $(call my-dir)

######################################################
# MobilePrintSvc_CUPS_Backend
include $(CLEAR_VARS)   
LOCAL_MODULE := MobilePrintSvc_CUPS_Backend
LOCAL_MODULE_TAGS := optional
LOCAL_SRC_FILES := MobilePrintSvc_CUPS_Backend.apk
LOCAL_MODULE_CLASS := APPS
LOCAL_MODULE_SUFFIX := $(COMMON_ANDROID_PACKAGE_SUFFIX)
LOCAL_CERTIFICATE := PRESIGNED
LOCAL_REQUIRED_MODULES := \
	libcups \
	libcupsdevfileusb \
	libcupsimage \
	libcupsusb\
	libcupsgs \
	libcupsimgtops \
	libinvCupsjni \
	libcupsinvgs \
	libcupsipp \
	libcupspng \
	libcupspstops \
	libcupssocket \
	libinvusb

include $(BUILD_PREBUILT)
#####################################################

include $(CLEAR_VARS)
LOCAL_MODULE_TAGS := optional
ifeq ($(TARGET_ARCH),x86)
	LOCAL_PREBUILT_LIBS := x86/libcups.so
else
	LOCAL_PREBUILT_LIBS := arm/libcups.so
endif
include $(BUILD_MULTI_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE_TAGS := optional
ifeq ($(TARGET_ARCH),x86)
	LOCAL_PREBUILT_LIBS := x86/libcupsdevfileusb.so
else
	LOCAL_PREBUILT_LIBS := arm/libcupsdevfileusb.so
endif
include $(BUILD_MULTI_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE_TAGS := optional
ifeq ($(TARGET_ARCH),x86)
	LOCAL_PREBUILT_LIBS := x86/libcupsimage.so
else
	LOCAL_PREBUILT_LIBS := arm/libcupsimage.so
endif
include $(BUILD_MULTI_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE_TAGS := optional
ifeq ($(TARGET_ARCH),x86)
	LOCAL_PREBUILT_LIBS := x86/libcupsusb.so
else
	LOCAL_PREBUILT_LIBS := arm/libcupsusb.so
endif
include $(BUILD_MULTI_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE_TAGS := optional
ifeq ($(TARGET_ARCH),x86)
	LOCAL_PREBUILT_LIBS := x86/libcupsgs.so
else
	LOCAL_PREBUILT_LIBS := arm/libcupsgs.so
endif
include $(BUILD_MULTI_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE_TAGS := optional
ifeq ($(TARGET_ARCH),x86)
	LOCAL_PREBUILT_LIBS := x86/libcupsimgtops.so
else
	LOCAL_PREBUILT_LIBS := arm/libcupsimgtops.so
endif
include $(BUILD_MULTI_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE_TAGS := optional
ifeq ($(TARGET_ARCH),x86)
	LOCAL_PREBUILT_LIBS := x86/libinvCupsjni.so
else
	LOCAL_PREBUILT_LIBS := arm/libinvCupsjni.so
endif
include $(BUILD_MULTI_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE_TAGS := optional
ifeq ($(TARGET_ARCH),x86)
	LOCAL_PREBUILT_LIBS := x86/libcupsinvgs.so
else
	LOCAL_PREBUILT_LIBS := arm/libcupsinvgs.so
endif
include $(BUILD_MULTI_PREBUILT)

#include $(CLEAR_VARS)
#LOCAL_MODULE_TAGS := optional
#LOCAL_PREBUILT_LIBS := libinvpoppler.so
#include $(BUILD_MULTI_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE_TAGS := optional
ifeq ($(TARGET_ARCH),x86)
	LOCAL_PREBUILT_LIBS := x86/libcupsipp.so
else
	LOCAL_PREBUILT_LIBS := arm/libcupsipp.so
endif
include $(BUILD_MULTI_PREBUILT)

#include $(CLEAR_VARS)
#LOCAL_MODULE_TAGS := optional
#LOCAL_PREBUILT_LIBS := libs/liblcms.so
#include $(BUILD_MULTI_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE_TAGS := optional
ifeq ($(TARGET_ARCH),x86)
	LOCAL_PREBUILT_LIBS := x86/libcupspng.so
else
	LOCAL_PREBUILT_LIBS := arm/libcupspng.so
endif
include $(BUILD_MULTI_PREBUILT)

#include $(CLEAR_VARS)
#LOCAL_MODULE_TAGS := optional
#LOCAL_PREBUILT_LIBS := libpoppler.so
#include $(BUILD_MULTI_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE_TAGS := optional
ifeq ($(TARGET_ARCH),x86)
	LOCAL_PREBUILT_LIBS := x86/libcupspstops.so
else
	LOCAL_PREBUILT_LIBS := arm/libcupspstops.so
endif
include $(BUILD_MULTI_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE_TAGS := optional
ifeq ($(TARGET_ARCH),x86)
	LOCAL_PREBUILT_LIBS := x86/libcupssocket.so
else
	LOCAL_PREBUILT_LIBS := arm/libcupssocket.so
endif
include $(BUILD_MULTI_PREBUILT)

include $(CLEAR_VARS)
LOCAL_MODULE_TAGS := optional
ifeq ($(TARGET_ARCH),x86)
	LOCAL_PREBUILT_LIBS := x86/libinvusb.so
else
	LOCAL_PREBUILT_LIBS := arm/libinvusb.so
endif
include $(BUILD_MULTI_PREBUILT)

#include $(CLEAR_VARS)
#LOCAL_MODULE_TAGS := optional
#LOCAL_PREBUILT_LIBS := libs/libpostprocess.so
#include $(BUILD_MULTI_PREBUILT)
