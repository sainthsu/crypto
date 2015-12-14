LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_LDLIBS :=-llog \
               -ldl \

LOCAL_MODULE    := crypto_static
LOCAL_SRC_FILES := Data.cpp \
		           Base64.cpp \
				   Crypto.cpp
				   
LOCAL_STATIC_LIBRARIES := crypto_static
include $(BUILD_SHARED_LIBRARY)

