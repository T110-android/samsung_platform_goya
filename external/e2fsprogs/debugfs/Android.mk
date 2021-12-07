LOCAL_PATH := $(call my-dir)

#########################
# Build the debugfs binary

debugfs_src_files :=  \
	debug_cmds.c \
	debugfs.c \
	util.c \
	ls.c \
	ncheck.c \
	icheck.c \
	lsdel.c \
	dump.c \
	set_fields.c \
	logdump.c \
	htree.c \
	unused.c \
	filefrag.c \
	../misc/e2freefrag.c


debugfs_shared_libraries := \
	libext2fs \
	libext2_blkid \
	libext2_uuid \
	libext2_profile \
	libext2_com_err \
	libext2_e2p \
	libext2_quota \
	libext2_ss

debugfs_system_shared_libraries := libc

debugfs_c_includes := external/e2fsprogs/lib

debugfs_cflags := -O2 -g -W -Wall \
	-DHAVE_DIRENT_H \
	-DHAVE_ERRNO_H \
	-DHAVE_INTTYPES_H \
	-DHAVE_LINUX_FD_H \
	-DHAVE_NETINET_IN_H \
	-DHAVE_SETJMP_H \
	-DHAVE_SYS_IOCTL_H \
	-DHAVE_SYS_MMAN_H \
	-DHAVE_SYS_MOUNT_H \
	-DHAVE_SYS_PRCTL_H \
	-DHAVE_SYS_RESOURCE_H \
	-DHAVE_SYS_SELECT_H \
	-DHAVE_SYS_STAT_H \
	-DHAVE_SYS_TYPES_H \
	-DHAVE_STDLIB_H \
	-DHAVE_UNISTD_H \
	-DHAVE_UTIME_H \
	-DHAVE_STRDUP \
	-DHAVE_MMAP \
	-DHAVE_GETPAGESIZE \
	-DHAVE_LSEEK64 \
	-DHAVE_LSEEK64_PROTOTYPE \
	-DHAVE_EXT2_IOCTLS \
	-DHAVE_TYPE_SSIZE_T \
	-DHAVE_INTPTR_T \
	-DENABLE_HTREE=1 \
	-DHAVE_SYS_TIME_H \
	-DHAVE_SYSCONF \
	-DHAVE_SIGNAL_H\
	-DDEBUGFS

include $(CLEAR_VARS)

LOCAL_SRC_FILES := $(debugfs_src_files)
LOCAL_C_INCLUDES := $(debugfs_c_includes)
LOCAL_CFLAGS := $(debugfs_cflags)
LOCAL_SYSTEM_SHARED_LIBRARIES := $(debugfs_system_shared_libraries)
LOCAL_SHARED_LIBRARIES := $(debugfs_shared_libraries)
LOCAL_MODULE := debugfs
LOCAL_MODULE_TAGS := optional

include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := $(debugfs_src_files)
LOCAL_C_INCLUDES := $(debugfs_c_includes)
LOCAL_CFLAGS := $(debugfs_cflags)
LOCAL_SHARED_LIBRARIES := $(addsuffix _host, $(debugfs_shared_libraries))
LOCAL_MODULE := debugfs_host
LOCAL_MODULE_STEM := debugfs
LOCAL_MODULE_TAGS := optional

include $(BUILD_HOST_EXECUTABLE)
