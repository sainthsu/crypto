#ifndef HEADER_CONFIG_H
#define HEADER_CONFIG_H

#define DEBUG 1

#ifdef DEBUG
#include <android/log.h>

#if defined(__x86_64__)||defined(__aarch64__)||defined(__mips64)
#define BIT64
#endif

#define LOG_TAG "Crypto_JNI"
#define LOG(fmt, args...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##args)

#endif

#endif

