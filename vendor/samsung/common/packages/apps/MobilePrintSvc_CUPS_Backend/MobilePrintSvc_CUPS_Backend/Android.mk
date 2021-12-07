LOCAL_PATH:= $(call my-dir)
MY_LOCAL_PATH := $(LOCAL_PATH)
include $(CLEAR_VARS)

LOCAL_PACKAGE_NAME   := MobilePrintSvc_CUPS_Backend
LOCAL_MODULE_TAGS    := optional
LOCAL_MANIFEST_FILE  := AndroidManifest.xml

LOCAL_SRC_FILES      := $(call all-java-files-under, src)\
	/src/com/siso/app/print/ipc/GPrintBackend.aidl \
	/src/com/siso/app/print/ipc/GPrintJobStatus.aidl \
	/src/com/siso/app/print/ipc/GPrintUsbResponse.aidl

			
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

include $(BUILD_PACKAGE)
######################################
# Use the folloing include to make our backend libraries
include $(MY_LOCAL_PATH)/native/libpng/Android.mk
include $(MY_LOCAL_PATH)/native/libusb-1.0.8/Android.mk
include $(MY_LOCAL_PATH)/native/gnu-ghostscript-8.64.0/Android.mk
include $(MY_LOCAL_PATH)/native/cups/Android.mk




