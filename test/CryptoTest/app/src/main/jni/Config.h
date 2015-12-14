#ifndef HEADER_CONFIG_H
#define HEADER_CONFIG_H

#define DEBUG 1

#ifdef DEBUG
#include <android/log.h>

#if defined(__LP64__)
#define BIT64
#endif

#define LOG_TAG "Crypto_JNI"
#define LOG(fmt, args...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##args)

#endif

#ifndef CRYPTO_NS_BEGIN
#define CRYPTO_NS_BEGIN namespace crypto {
#endif

#ifndef CRYPTO_NS_END
#define CRYPTO_NS_END }
#endif

#endif

