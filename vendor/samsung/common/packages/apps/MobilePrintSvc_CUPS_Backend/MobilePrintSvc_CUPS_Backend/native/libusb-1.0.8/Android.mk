LOCAL_PATH:= $(call my-dir)
########################################

include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional

LOCAL_SRC_FILES:= \
		libusb/core.c \
 		libusb/descriptor.c \
 		libusb/io.c \
 		libusb/sync.c \
 		libusb/os/linux_usbfs.c \
		lusb/core.c #added

LOCAL_C_INCLUDES += \
		$(LOCAL_PATH) \
		$(LOCAL_PATH)/libusb \
		$(LOCAL_PATH)/libusb/os \
 		$(LOCAL_PATH)/lusb
		
LOCAL_C_INCLUDES += $(JNI_H_INCLUDE)

LOCAL_LDLIBS:= -llog

LOCAL_SHARED_LIBRARIES := liblog libnativehelper

LOCAL_MODULE:= libcupsusb


include $(BUILD_SHARED_LIBRARY)

#######################################

#include $(CLEAR_VARS)

#LOCAL_MODULE_TAGS := eng

#LOCAL_SRC_FILES:= examples/lsusb.c
#LOCAL_MODULE := lsusb

#LOCAL_C_INCLUDES += \
		$(LOCAL_PATH) \
		$(LOCAL_PATH)/examples

#LOCAL_SHARED_LIBRARIES := libusb

#include $(BUILD_EXECUTABLE)
