LOCAL_PATH:= $(call my-dir)

common_CFLAGS := $(common_CCFLAGS) \
	-Wstrict-prototypes \
	-Wmissing-prototypes \
	-Wnested-externs \
	-Wbad-function-cast \
	-std=c99

common_C_INCLUDES := \
	$(LOCAL_PATH)/../ \

# the delegate of CPU
# =====================================================
include $(CLEAR_VARS)
LOCAL_MODULE := delegate_cpu
LOCAL_C_INCLUDES := \
	external/libxml2/include \
	external/icu4c/common \
	$(common_C_INCLUDES)

LOCAL_SRC_FILES:= \
	$(LOCAL_MODULE).c \
	sysfs_cpu.c
LOCAL_CFLAGS := $(common_CFLAGS)

LOCAL_SHARED_LIBRARIES := libsysfs_attribute
LOCAL_ALLOW_UNDEFINED_SYMBOLS := true
LOCAL_LDFLAGS := -Wl,--allow-shlib-undefined

LOCAL_PRELINK_MODULE := false
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_SHARED_LIBRARIES)/PowerDaemon
LOCAL_UNSTRIPPED_PATH := $(TARGET_OUT_SHARED_LIBRARIES_UNSTRIPPED)/PowerDaemon
include $(BUILD_SHARED_LIBRARY)

# the delegate of DDR
# =====================================================
include $(CLEAR_VARS)
LOCAL_MODULE := delegate_ddr
LOCAL_C_INCLUDES := \
	external/libxml2/include \
	external/icu4c/common \
	$(common_C_INCLUDES)

LOCAL_SRC_FILES:= \
	$(LOCAL_MODULE).c \
	sysfs_ddr.c
LOCAL_CFLAGS := $(common_CFLAGS)

LOCAL_SHARED_LIBRARIES := libsysfs_attribute
LOCAL_ALLOW_UNDEFINED_SYMBOLS := true
LOCAL_LDFLAGS := -Wl,--allow-shlib-undefined

LOCAL_PRELINK_MODULE := false
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_SHARED_LIBRARIES)/PowerDaemon
LOCAL_UNSTRIPPED_PATH := $(TARGET_OUT_SHARED_LIBRARIES_UNSTRIPPED)/PowerDaemon
include $(BUILD_SHARED_LIBRARY)

# the delegate of GPU
# =====================================================
include $(CLEAR_VARS)
LOCAL_MODULE := delegate_gpu
LOCAL_C_INCLUDES := \
	external/libxml2/include \
	external/icu4c/common \
	$(common_C_INCLUDES)

LOCAL_SRC_FILES:= \
	$(LOCAL_MODULE).c \
	sysfs_gpu.c
LOCAL_CFLAGS := $(common_CFLAGS)

LOCAL_SHARED_LIBRARIES := libsysfs_attribute
LOCAL_ALLOW_UNDEFINED_SYMBOLS := true
LOCAL_LDFLAGS := -Wl,--allow-shlib-undefined

LOCAL_PRELINK_MODULE := false
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_SHARED_LIBRARIES)/PowerDaemon
LOCAL_UNSTRIPPED_PATH := $(TARGET_OUT_SHARED_LIBRARIES_UNSTRIPPED)/PowerDaemon
include $(BUILD_SHARED_LIBRARY)

# the delegate of SHELL CMD
# =====================================================
include $(CLEAR_VARS)
LOCAL_MODULE := delegate_shcmd
LOCAL_C_INCLUDES := \
	external/libxml2/include \
	external/icu4c/common \
	$(common_C_INCLUDES)

LOCAL_SRC_FILES:= \
	$(LOCAL_MODULE).c
LOCAL_CFLAGS := $(common_CFLAGS)

LOCAL_ALLOW_UNDEFINED_SYMBOLS := true
LOCAL_LDFLAGS := -Wl,--allow-shlib-undefined

LOCAL_PRELINK_MODULE := false
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_SHARED_LIBRARIES)/PowerDaemon
LOCAL_UNSTRIPPED_PATH := $(TARGET_OUT_SHARED_LIBRARIES_UNSTRIPPED)/PowerDaemon
include $(BUILD_SHARED_LIBRARY)

# libsysfs
# =====================================================
include $(CLEAR_VARS)
LOCAL_MODULE := libsysfs_attribute
LOCAL_SRC_FILES:= \
        libsysfs.c
LOCAL_CFLAGS := $(common_CFLAGS)
LOCAL_ALLOW_UNDEFINED_SYMBOLS := true
LOCAL_LDFLAGS := -Wl,--allow-shlib-undefined
LOCAL_PRELINK_MODULE := false
LOCAL_MODULE_TAGS := optional
include $(BUILD_SHARED_LIBRARY)
