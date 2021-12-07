LOCAL_PATH := $(call my-dir)

libext2_ss_src_files := \
	invocation.c \
	help.c \
	execute_cmd.c \
	listen.c \
	parse.c \
	error.c \
	prompt.c \
	request_tbl.c \
	list_rqs.c \
	pager.c \
	requests.c \
	data.c \
	get_readline.c \
	ss_err.c \
	std_rqs.c


libext2_ss_c_includes := external/e2fsprogs/lib

libext2_ss_cflags := -O2 -g -W -Wall \
	-DHAVE_INTTYPES_H \
	-DHAVE_UNISTD_H \
	-DHAVE_ERRNO_H \
	-DHAVE_NETINET_IN_H \
	-DHAVE_SYS_IOCTL_H \
	-DHAVE_SYS_MMAN_H \
	-DHAVE_SYS_MOUNT_H \
	-DHAVE_SYS_PRCTL_H \
	-DHAVE_SYS_RESOURCE_H \
	-DHAVE_SYS_SELECT_H \
	-DHAVE_SYS_STAT_H \
	-DHAVE_SYS_TYPES_H \
	-DHAVE_STDLIB_H \
	-DHAVE_STRDUP \
	-DHAVE_MMAP \
	-DHAVE_UTIME_H \
	-DHAVE_GETPAGESIZE \
	-DHAVE_LSEEK64 \
	-DHAVE_LSEEK64_PROTOTYPE \
	-DHAVE_EXT2_IOCTLS \
	-DHAVE_LINUX_FD_H \
	-DHAVE_TYPE_SSIZE_T \
	-DHAVE_SYS_TIME_H \
	-DHAVE_SYSCONF \
	-DHAVE_DIRENT_H

libext2_ss_shared_libraries := libext2_com_err

libext2_ss_system_shared_libraries := libc

include $(CLEAR_VARS)

LOCAL_SRC_FILES := $(libext2_ss_src_files)
LOCAL_SHARED_LIBRARIES := $(libext2_ss_shared_libraries)
LOCAL_SYSTEM_SHARED_LIBRARIES := $(libext2_ss_system_shared_libraries)
LOCAL_C_INCLUDES := $(libext2_ss_c_includes)
LOCAL_CFLAGS := $(libext2_ss_cflags)
LOCAL_MODULE := libext2_ss
LOCAL_MODULE_TAGS := optional
LOCAL_PRELINK_MODULE := false

include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := $(libext2_ss_src_files)
LOCAL_SHARED_LIBRARIES := $(addsuffix _host, $(libext2_ss_shared_libraries))
LOCAL_C_INCLUDES := $(libext2_ss_c_includes)
LOCAL_CFLAGS := $(libext2_ss_cflags)
LOCAL_MODULE := libext2_ss_host
LOCAL_MODULE_TAGS := optional
LOCAL_PRELINK_MODULE := false

include $(BUILD_HOST_SHARED_LIBRARY)
