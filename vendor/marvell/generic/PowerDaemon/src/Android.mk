LOCAL_PATH:= $(call my-dir)

common_CFLAGS := -O2 -W -Wall -Wshadow -W -Wpointer-arith \
	-Wcast-qual -Wcast-align -Wwrite-strings \
	-Wmissing-declarations -Wredundant-decls \
	-Wnested-externs -Winline -Wundef -Wbad-function-cast \
	-Waggregate-return \
	-std=c99

common_CFLAGS += -D_POSIX_SOURCE -D_GNU_SOURCE

common_C_INCLUDES := \
	$(LOCAL_PATH)/

include $(CLEAR_VARS)
LOCAL_C_INCLUDES := \
	external/libxml2/include \
	external/icu4c/common \
	$(common_C_INCLUDES)

LOCAL_SRC_FILES:= \
	config_parser.c \
	ppd_log.c \
	daemon_utils.c \
	list.c \
	main.c \
	ppd_plugin_utils.c \
	ppd_delegate_utils.c \
	sock_utils.c

LOCAL_SHARED_LIBRARIES := libdl libxml2 libcutils
LOCAL_CFLAGS := $(common_CFLAGS)
LOCAL_LDFLAGS := -Wl,--export-dynamic

LOCAL_MODULE_PATH := $(TARGET_OUT_EXECUTABLES)
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := powerdaemon
include $(BUILD_EXECUTABLE)

include $(call all-makefiles-under,$(LOCAL_PATH))
