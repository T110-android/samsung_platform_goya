LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= \
	lib/cpufreq.c \
	lib/sysfs.c

common_CFLAGS := -pipe
common_CFLAGS += -DVERSION=\"$(VERSION)\" -DPACKAGE=\"$(PACKAGE)\" \
		-DPACKAGE_BUGREPORT=\"$(PACKAGE_BUGREPORT)\"

WARNINGS := -Wall -Wchar-subscripts -Wpointer-arith -Wsign-compare
WARNINGS += $(call cc-supports,-Wno-pointer-sign)
WARNINGS += $(call cc-supports,-Wdeclaration-after-statement)
WARNINGS += -Wshadow

# use '-Os' optimization if available, else use -O2
OPTIMIZATION := $(call cc-supports,-Os,-O2)

common_CFLAGS += $(WARNINGS)
common_CFLAGS += $(OPTIMIZATION) -fomit-frame-pointer
common_CFLAGS += -D_POSIX_SOURCE -D_GNU_SOURCE

common_C_INCLUDES += \
	$(LOCAL_PATH)/lib

LOCAL_CFLAGS := $(common_CFLAGS)
LOCAL_C_INCLUDES := \
	$(common_C_INCLUDES)
#
# define LOCAL_PRELINK_MODULE to false to not use pre-link map
#
LOCAL_PRELINK_MODULE := false

LOCAL_MODULE := libcpufreq
LOCAL_MODULE_TAGS := optional
include $(BUILD_SHARED_LIBRARY)

# the util of cpufreq-aperf
# =====================================================
include $(CLEAR_VARS)

LOCAL_CFLAGS := $(common_CFLAGS)
LOCAL_C_INCLUDES := \
	$(common_C_INCLUDES)
LOCAL_SHARED_LIBRARIES := libcpufreq

LOCAL_SRC_FILES:= \
	utils/aperf.c

LOCAL_MODULE_PATH := $(TARGET_OUT_OPTIONAL_EXECUTABLES)
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := cpufreq-aperf
include $(BUILD_EXECUTABLE)

# the util of cpufreq-info
# =====================================================
include $(CLEAR_VARS)

LOCAL_CFLAGS := $(common_CFLAGS)
LOCAL_C_INCLUDES := \
	$(common_C_INCLUDES)
LOCAL_SHARED_LIBRARIES := libcpufreq

LOCAL_SRC_FILES:= \
	utils/info.c

LOCAL_MODULE_PATH := $(TARGET_OUT_OPTIONAL_EXECUTABLES)
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := cpufreq-info
include $(BUILD_EXECUTABLE)

# the util of cpufreq-set
# =====================================================
include $(CLEAR_VARS)

LOCAL_CFLAGS := $(common_CFLAGS)
LOCAL_C_INCLUDES := \
	$(common_C_INCLUDES)
LOCAL_SHARED_LIBRARIES := libcpufreq

LOCAL_SRC_FILES:= \
	utils/set.c

LOCAL_MODULE_PATH := $(TARGET_OUT_OPTIONAL_EXECUTABLES)
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := cpufreq-set
include $(BUILD_EXECUTABLE)
