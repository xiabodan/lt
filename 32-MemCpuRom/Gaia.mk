# Copyright 2006 The Android Open Source Project
ifeq ($(TARGET_ARCH),arm)
LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_SRC_FILES:= \
        MemoryTest.c
LOCAL_SHARED_LIBRARIES := libc
LOCAL_MODULE:= MemoryTest
LOCAL_MODULE_TAGS := optional
## LOCAL_CFLAGS += -fstack-protector-all
LOCAL_CFLAGS += -fomit-frame-pointer
LOCAL_C_INCLUDES += bionic
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_SRC_FILES:= \
        RomTest.c
LOCAL_SHARED_LIBRARIES := libc
LOCAL_MODULE:= RomTest
LOCAL_MODULE_TAGS := optional
## LOCAL_CFLAGS += -fstack-protector-all
LOCAL_CFLAGS += -fomit-frame-pointer
LOCAL_C_INCLUDES += bionic
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_SRC_FILES:= \
        TaskTest.c
LOCAL_SHARED_LIBRARIES := libc
LOCAL_MODULE:= TaskTest
LOCAL_MODULE_TAGS := optional
## LOCAL_CFLAGS += -fstack-protector-all
LOCAL_CFLAGS += -fomit-frame-pointer
LOCAL_C_INCLUDES += bionic
include $(BUILD_EXECUTABLE)

endif
