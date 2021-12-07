LOCAL_PATH:=$(call my-dir)

#########################################
# Build the libcups.so library

include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional

cups_src_files := \
		cups/adminutil.c \
                cups/array.c \
                cups/attr.c \
                cups/auth.c \
                cups/backchannel.c \
                cups/backend.c \
                cups/conflicts.c \
                cups/custom.c \
                cups/debug.c \
                cups/dest.c \
                cups/dir.c \
                cups/emit.c \
                cups/encode.c \
                cups/file.c \
                cups/getdevices.c \
                cups/getifaddrs.c \
                cups/getputfile.c \
                cups/globals.c \
                cups/http.c \
                cups/http-addr.c \
                cups/http-addrlist.c \
                cups/http-support.c \
                cups/ipp.c \
                cups/ipp-support.c \
                cups/langprintf.c \
                cups/language.c \
                cups/localize.c \
                cups/mark.c \
                cups/md5.c \
                cups/md5passwd.c \
                cups/notify.c \
                cups/options.c \
                cups/page.c \
                cups/ppd.c \
                cups/ppd-cache.c \
                cups/pwg-media.c \
                cups/request.c \
                cups/sidechannel.c \
                cups/snmp.c \
                cups/snprintf.c \
                cups/string.c \
                cups/tempfile.c \
                cups/thread.c \
                cups/transcode.c \
                cups/usersys.c \
                cups/util.c


LOCAL_SRC_FILES := $(cups_src_files)

#LOCAL_LDLIBS := -lc -lz -lcrypto -lm

#LOCAL_PREBUILT_LIBS := $(LOCAL_PATH)/cup/libcups.so
LOCAL_C_INCLUDES := \
		$(LOCAL_PATH) \
		$(LOCAL_PATH)/cups\
		external/zlib
LOCAL_SHARED_LIBRARIES:= libcutils\
                         libdbus\
                         libz

LOCAL_LDLIBS += -lc -lz -lm
LOCAL_PRELINK_MODULE := false
LOCAL_MODULE:= libcups

include $(BUILD_SHARED_LIBRARY)
########################################


# Build the libcupimage.so library

include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional
 
filter_src_files := \
		filter/image-bmp.c \
		filter/image-colorspace.c \
		filter/image-gif.c \
		filter/image-jpeg.c \
		filter/image-photocd.c \
		filter/image-pix.c \
		filter/image-png.c \
		filter/image-pnm.c \
		filter/image-sgi.c \
		filter/image-sgilib.c \
		filter/image-sun.c \
		filter/image-tiff.c \
		filter/image-zoom.c \
		filter/image.c \
		filter/error.c \
		filter/interpret.c \
		filter/raster.c

LOCAL_SRC_FILES := $(filter_src_files)

LOCAL_SHARED_LIBRARIES := libjpeg \
			  libcups \
		          libz \
			liblog \
			libcupspng

LOCAL_LDLIBS := -llog -lz

//LOCAL_STATIC_LIBRARIES := libpng

LOCAL_C_INCLUDES := \
                $(LOCAL_PATH) \
                $(LOCAL_PATH)/cups \
               	external/jpeg \
                external/libpng \
				external/zlib

LOCAL_LDLIBS += -lc -lz -lm
LOCAL_PRELINK_MODULE := false
LOCAL_MODULE:=libcupsimage

include $(BUILD_SHARED_LIBRARY)

#############################
# Build the libbackend.a library
include $(CLEAR_VARS)
LOCAL_MODULE_TAGS := optional
libbackend_src_files := \
		backend/ieee1284.c \
		backend/network.c \
		backend/runloop.c \
		backend/snmp-supplies.c

LOCAL_SRC_FILES := $(libbackend_src_files)

LOCAL_C_INCLUDES := \
                $(LOCAL_PATH) \
                $(LOCAL_PATH)/backend \
				$(LOCAL_PATH)/cups
		
LOCAL_LDLIBS += -lc -lz -lm
LOCAL_PRELINK_MODULE := false
LOCAL_MODULE:= libbackend
include $(BUILD_STATIC_LIBRARY)

##############################
# Build the backend "ipp"

include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional

ipp_src_files := backend/ipp.c

LOCAL_SRC_FILES := $(ipp_src_files)

LOCAL_C_INCLUDES := \
                $(LOCAL_PATH) \
				$(LOCAL_PATH)/backend \
				$(LOCAL_PATH)/cups

LOCAL_SHARED_LIBRARIES := \
			libcups liblog
LOCAL_STATIC_LIBRARIES := libbackend

LOCAL_LDLIBS := -lc -lz -lm -llog

LOCAL_PRELINK_MODULE := false

LOCAL_MODULE := libcupsipp

include $(BUILD_SHARED_LIBRARY)


###########################################
# Build the backend "socket"

include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional

ipp_src_files := backend/socket.c

LOCAL_SRC_FILES := $(ipp_src_files)

LOCAL_C_INCLUDES := \
                $(LOCAL_PATH) \
				$(LOCAL_PATH)/backend \
				$(LOCAL_PATH)/cups

LOCAL_SHARED_LIBRARIES := \
			 libcups liblog
LOCAL_STATIC_LIBRARIES := libbackend

LOCAL_LDLIBS := -lc -lz -lm -llog

LOCAL_PRELINK_MODULE := false

LOCAL_MODULE := libcupssocket

include $(BUILD_SHARED_LIBRARY)


###########################################

include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional

LOCAL_SRC_FILES:=filter/imagetops.c \
				 filter/common.c

LOCAL_C_INCLUDES := \
                $(LOCAL_PATH) \
                $(LOCAL_PATH)/cups \
                $(LOCAL_PATH)/filter \
		$(LOCAL_PATH)/backend

LOCAL_SHARED_LIBRARIES := \
                        libcupsimage \
			libcups \
			liblog \
			
						
LOCAL_LDLIBS := -lz -lm -llog

LOCAL_PRELINK_MODULE := false

LOCAL_MODULE:=libcupsimgtops

include $(BUILD_SHARED_LIBRARY)
###############################
#Build "usb" backend lib (dev/usb/lp0)

include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional

usb_src_files := backend/usb-unix.c

LOCAL_SRC_FILES := $(usb_src_files)

LOCAL_C_INCLUDES := \
                $(LOCAL_PATH) \
				$(LOCAL_PATH)/cups \
                $(LOCAL_PATH)/backend \
			  
LOCAL_STATIC_LIBRARIES := libbackend

LOCAL_SHARED_LIBRARIES := \
                        libcups liblog libnativehelper 

LOCAL_PRELINK_MODULE := false

LOCAL_MODULE := libcupsdevfileusb

include $(BUILD_SHARED_LIBRARY)

#######################################
#Build USB backend using libusb
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional

LOCAL_SRC_FILES:=backend/usb-libusb.c

LOCAL_C_INCLUDES := \
                $(LOCAL_PATH) \
                $(LOCAL_PATH)/cups \
                $(LOCAL_PATH)/filter \
				$(LOCAL_PATH)/backend \
				$(LOCAL_PATH)/../libusb-1.0.8/lusb
		
LOCAL_STATIC_LIBRARIES := libbackend

LOCAL_SHARED_LIBRARIES := liblog libnativehelper libandroid_runtime libcups libcupsusb

LOCAL_PRELINK_MODULE := false

LOCAL_MODULE:=libinvusb

include $(BUILD_SHARED_LIBRARY)

#######################################
#Build "invPrinterSearch" backend lib

#include $(CLEAR_VARS)

#LOCAL_MODULE_TAGS := optional

#usb_src_files := backend/snmp.c

#LOCAL_SRC_FILES := $(usb_src_files)

#LOCAL_C_INCLUDES := \
                $(LOCAL_PATH) \
				$(LOCAL_PATH)/cups \
                $(LOCAL_PATH)/backend 

#LOCAL_STATIC_LIBRARIES := libbackend

#LOCAL_SHARED_LIBRARIES := \
                        libcups liblog libnativehelper libandroid_runtime

#LOCAL_PRELINK_MODULE := false

#LOCAL_MODULE := libinvPrinterSearch

#include $(BUILD_SHARED_LIBRARY)
####################################
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional

LOCAL_SRC_FILES:=invokeCups.cpp

LOCAL_C_INCLUDES := \
                $(LOCAL_PATH) \
                $(LOCAL_PATH)/cups \
                $(LOCAL_PATH)/filter \
		$(LOCAL_PATH)/backend \
		
LOCAL_LDLIBS:= -llog

LOCAL_SHARED_LIBRARIES := liblog libnativehelper libandroid_runtime libcutils libcupsimgtops libcupsipp libcupssocket libinvusb libcupsdevfileusb

LOCAL_MODULE:=libinvCupsjni

include $(BUILD_SHARED_LIBRARY)
######################################

# build  pstops shared library
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional

LOCAL_SRC_FILES:=filter/pstops.c \
				 filter/common.c


LOCAL_C_INCLUDES := \
                $(LOCAL_PATH) \
                $(LOCAL_PATH)/cups

LOCAL_SHARED_LIBRARIES := \
                        libcups \
						libcutils

LOCAL_LDLIBS := -lz -lcrypto -ldbus -lthread_db -lm -llog

LOCAL_PRELINK_MODULE := false

LOCAL_MODULE := libcupspstops

include $(BUILD_SHARED_LIBRARY)
###########################################

# build  libinvpoppler shared library

#include $(CLEAR_VARS)

#LOCAL_MODULE_TAGS := optional

#LOCAL_CPP_EXTENSION := .cc
#LOCAL_SRC_FILES:= filter/pdftops.cc


#LOCAL_C_INCLUDES := \
#                $(LOCAL_PATH) \
#                $(LOCAL_PATH)/cups \
#				$(LOCAL_PATH)/../poppler-0.22.1/utils

#LOCAL_SHARED_LIBRARIES := \
#				libcups \
#				libpoppler \
#				libpstops \
#				libcutils \
#				libnativehelper

#LOCAL_LDLIBS := -lz -lcrypto -ldbus -lthread_db -lm -llog

#LOCAL_PRELINK_MODULE := false

#LOCAL_MODULE := libinvpoppler

#include $(BUILD_SHARED_LIBRARY)
##########################################

include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional

LOCAL_SRC_FILES:=invokeGhostScript.cpp

LOCAL_C_INCLUDES := \
                $(LOCAL_PATH) \
                $(LOCAL_PATH)/../gnu-ghostscript-8.64.0/ \
		$(LOCAL_PATH)/../gnu-ghostscript-8.64.0/psi

LOCAL_SHARED_LIBRARIES := liblog  libcutils libcupsgs libnativehelper 

LOCAL_MODULE:=libcupsinvgs

include $(BUILD_SHARED_LIBRARY)

############################################



