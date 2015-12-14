#include "JNIBridge.h"
#include "Crypto.h"
#include "Base64.h"
#include <string.h>

static Crypto *crypto = NULL;

static Data& byteArrayToData(JNIEnv* env,jbyteArray array) {
    int length = env->GetArrayLength(array);
    char* bytes = (char*)env->GetByteArrayElements(array,JNI_FALSE);
    char* data = (char*)malloc(length);
    memcpy(data,bytes,length);
    Data* result = new Data();
    result->fastSet((unsigned char*)data,length);

    env->ReleaseByteArrayElements(array,(jbyte*)bytes,0);
    return *result;
}


static jbyteArray dataToByteArray(JNIEnv* env,Data& data) {
    if(data.isNull()) {
        return NULL;
    }

    int length = data.getSize();
    unsigned char* bytes = data.getBytes();

    jbyteArray array = env->NewByteArray(length);
    env->SetByteArrayRegion(array,0,length,(jbyte*)bytes);

    return array;
}




JNIEXPORT jbyteArray Java_org_feike_acrypto_Crypto_MD5(JNIEnv* env,jclass clazz,jbyteArray data)
{
    if(crypto == NULL) {
        crypto = new Crypto();
    }
    Data src = byteArrayToData(env,data);
    Data result = crypto->md5(src);

    jbyteArray array = dataToByteArray(env,result);

    return array;

}
/*
JNIEXPORT jbyteArray Java_org_feike_acrypto_Crypto_MD5(JNIEnv* env,jclass clazz,jstring filename)
{
    if(crypto == NULL) {
        crypto = new Crypto();
    }

    return NULL;
}*/

JNIEXPORT jbyteArray Java_org_feike_acrypto_Crypto_AESEncode(JNIEnv* env,jclass clazz,jbyteArray data,jbyteArray key)
{
    if(crypto == NULL) {
        crypto = new Crypto();
    }

    Data src = byteArrayToData(env,data);
    Data keyData = byteArrayToData(env,key);

    Data result = crypto->AESEncode(src,keyData);
    jbyteArray array = dataToByteArray(env,result);

    return array;

}

JNIEXPORT jbyteArray Java_org_feike_acrypto_Crypto_AESDecode(JNIEnv* env,jclass clazz,jbyteArray data,jbyteArray key)
{
    if(crypto == NULL) {
        crypto = new Crypto();
    }

    Data src = byteArrayToData(env,data);
    Data keyData = byteArrayToData(env,key);

    Data result = crypto->AESDecode(src,keyData);
    jbyteArray array = dataToByteArray(env,result);

    return array;

}

JNIEXPORT jbyteArray Java_org_feike_acrypto_Crypto_Base64Encode(JNIEnv* env,jclass clazz,jbyteArray data)
{
    if(crypto == NULL) {
        crypto = new Crypto();
    }

    Data src = byteArrayToData(env,data);
    Data result = base64_encode(src);

    jbyteArray array = dataToByteArray(env,result);

    return array;


}

JNIEXPORT jbyteArray Java_org_feike_acrypto_Crypto_Base64Decode(JNIEnv* env,jclass clazz,jbyteArray data)
{
    if(crypto == NULL) {
        crypto = new Crypto();
    }

    Data src = byteArrayToData(env,data);
    Data result = base64_decode(src);

    jbyteArray array = dataToByteArray(env,result);

    return array;
}

JNIEXPORT jbyteArray Java_org_feike_acrypto_Crypto_SHA(JNIEnv* env,jclass clazz,jbyteArray data,jint type)
{
    if(crypto == NULL) {
        crypto = new Crypto();
    }

    Data src = byteArrayToData(env,data);
    Data result = Data::Null;
    switch (type) {
        case SHA1:
            result = crypto->sha1(src);
            break;
        case SHA224:
            result = crypto->sha224(src);
            break;
        case SHA256:
            result = crypto->sha256(src);
            break;
        case SHA384:
            result = crypto->sha384(src);
            break;
        case SHA512:
            result = crypto->sha512(src);
            break;

    }

    jbyteArray array = dataToByteArray(env,result);

    return array;
}