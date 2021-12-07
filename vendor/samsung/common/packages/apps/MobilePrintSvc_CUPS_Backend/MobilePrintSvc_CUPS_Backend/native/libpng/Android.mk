LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

common_SRC_FILES := \
	png.c \
	pngerror.c \
	pnggccrd.c \
	pngget.c \
	pngmem.c \
	pngpread.c \
	pngread.c \
	pngrio.c \
	pngrtran.c \
	pngrutil.c \
	pngset.c \
	pngtrans.c \
	pngvcrd.c \
	pngwio.c \
	pngwrite.c \
	pngwtran.c \
	pngwutil.c

common_CFLAGS := -fvisibility=hidden ## -fomit-frame-pointer

ifeq ($(HOST_OS),windows)
  ifeq ($(USE_MINGW),)
    # Case where we're building windows but not under linux (so it must be cygwin)
    # In this case, gcc cygwin doesn't recognize -fvisibility=hidden
    $(info libpng: Ignoring gcc flag $(common_CFLAGS) on Cygwin)
    common_CFLAGS := 
  endif
endif

common_C_INCLUDES += 

common_COPY_HEADERS_TO := libpng
common_COPY_HEADERS := png.h pngconf.h pngusr.h


# For the device
# =====================================================

ifeq ($(ARCH_ARM_HAVE_NEON),true)
   common_SRC_FILES += contrib/pngneon/png_read_filter_row_neon.s
   common_CFLAGS += -D__ARM_HAVE_NEON
endif


include $(CLEAR_VARS)

LOCAL_SRC_FILES := $(common_SRC_FILES)
LOCAL_CFLAGS += $(common_CFLAGS)
LOCAL_C_INCLUDES += $(common_C_INCLUDES) \
        external/zlib
LOCAL_SHARED_LIBRARIES := \
        libz
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE:= libcupspng

LOCAL_PRELINK_MODULE := false

include $(BUILD_SHARED_LIBRARY)
