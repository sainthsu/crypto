LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_LDLIBS :=-llog \
               -ldl \

LOCAL_MODULE    := smssdk
LOCAL_SRC_FILES := Data.cpp \
				   Crypto.cpp
				   
LOCAL_STATIC_LIBRARIES := crypto_static
include $(BUILD_SHARED_LIBRARY)

