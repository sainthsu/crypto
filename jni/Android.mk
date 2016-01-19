LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_LDLIBS :=-llog \
               -ldl \

LOCAL_MODULE    := acrypto
LOCAL_SRC_FILES := Data.cpp \
		           Base64.cpp \
				   Crypto.cpp \
				   JNIBridge.cpp

include $(BUILD_SHARED_LIBRARY)

