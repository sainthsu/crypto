//
// Created by Steve Hsu on 14/12/2015.
//

#ifndef CRYPTOTEST_JNIBRIDGE_H
#define CRYPTOTEST_JNIBRIDGE_H

#include "jni.h"


enum SHAType {
    SHA1 = 1,
    SHA224,
    SHA256,
    SHA384,
    SHA512
};

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jbyteArray Java_org_feike_acrypto_Crypto_MD5(JNIEnv* env,jclass clazz,jbyteArray data);
//JNIEXPORT jbyteArray Java_org_feike_acrypto_Crypto_MD5(JNIEnv* env,jclass clazz,jstring filename);
JNIEXPORT jbyteArray Java_org_feike_acrypto_Crypto_AESEncode(JNIEnv* env,jclass clazz,jbyteArray data,jbyteArray key);
JNIEXPORT jbyteArray Java_org_feike_acrypto_Crypto_AESDecode(JNIEnv* env,jclass clazz,jbyteArray data,jbyteArray key);
JNIEXPORT jbyteArray Java_org_feike_acrypto_Crypto_Base64Encode(JNIEnv* env,jclass clazz,jbyteArray data);
JNIEXPORT jbyteArray Java_org_feike_acrypto_Crypto_Base64Decode(JNIEnv* env,jclass clazz,jbyteArray data);
JNIEXPORT jbyteArray Java_org_feike_acrypto_Crypto_SHA(JNIEnv* env,jclass clazz,jbyteArray data,jint type);


#ifdef __cplusplus
}
#endif

#endif //CRYPTOTEST_JNIBRIDGE_H
