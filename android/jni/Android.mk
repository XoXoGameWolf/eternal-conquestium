LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := coco
LOCAL_SRC_FILES := src/main.c
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include 
LOCAL_LDLIBS := -llog -lGLESv3 -lm

include $(BUILD_SHARED_LIBRARY)