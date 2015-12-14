/**
 * author Steve Hsu
 * steve@kunkua.com
 */
#include "Config.h"
#include "Crypto.h"

#include <stdlib.h>

#include <dlfcn.h>

#include "openssl/aes.h"
#include "openssl/md5.h"
#include "openssl/sha.h"
//#include <openssl/rsa.h>

Crypto::Crypto()
:aesKey(NULL)
,rsaPublicKey(NULL)
{
	openLib();
}

Crypto::~Crypto()
{
	if(dlHandler)
		dlclose(dlHandler);
}

void Crypto::openLib()
{
#ifdef BIT64
	dlHandler = dlopen("/system/lib64/libcrypto.so",RTLD_LAZY);
#else
	dlHandler = dlopen("/system/lib/libcrypto.so",RTLD_LAZY);
#endif
	if(dlHandler == NULL) {
        LOG("dlopen Error:%s", dlerror());
    }
}

/**
 * all char array must end of '\0'
 */
Data& Crypto::AESEncode(const Data &src,const Data &key)
{
    if(src.isNull() || key.isNull()) {
        return (Data &) Data::Null;
    }

	AES_set_encrypt_key setKey = NULL;
	AES_encrypt encrypt = NULL;

	if(dlHandler == NULL) {
		openLib();
	}

	if(dlHandler) {
		setKey = (AES_set_encrypt_key)dlsym(dlHandler,"AES_set_encrypt_key");
		encrypt = (AES_encrypt)dlsym(dlHandler,"AES_encrypt");
	}

	 AES_KEY aes_key;

	 if(setKey(key.getBytes(), key.getSize()*8, &aes_key) < 0) {
	      return (Data &) Data::Null;
	 }

	 unsigned int data_length = src.getSize();
	 unsigned int out_length = data_length;

	 //add PKCS7 Padding
	 //If the original data is a multiple of N bytes, then an extra block of bytes with value N is added
	 //http://stackoverflow.com/questions/3283787/size-of-data-after-aes-cbc-and-aes-ecb-encryption
	 int padding = 0;
	 unsigned int i = 0;
	 if (data_length % AES_BLOCK_SIZE > 0) {
	     i = padding =  AES_BLOCK_SIZE - data_length % AES_BLOCK_SIZE;
	 }
	 if(i == 0) {
		 i = AES_BLOCK_SIZE;
		 out_length = data_length + AES_BLOCK_SIZE;
	 } else {
		 out_length = data_length + padding;
	 }

	 LOG("out:%d,padding:%d,in:%d",out_length,padding,data_length);

	 unsigned char tmp[out_length + 1];
	 memcpy(tmp,src.getBytes(),data_length);
	 tmp[out_length] = '\0';

	 while (i > 0) {
	      tmp[data_length + i - 1] = 32;
	      i--;
	 }

	 unsigned char* out = (unsigned char*)malloc(out_length + 1);
	 out[out_length] = '\0';

	 unsigned char inBlock[AES_BLOCK_SIZE+1];
	 unsigned char outBlock[AES_BLOCK_SIZE];
	 int blockNum = out_length/AES_BLOCK_SIZE;
	 inBlock[AES_BLOCK_SIZE] = '\0';
	 for(i = 0; i < blockNum; i++) {
		 memcpy(&(inBlock[0]),&(tmp[AES_BLOCK_SIZE*i]),AES_BLOCK_SIZE);
		 memset(&(outBlock[0]),0,AES_BLOCK_SIZE);
		 encrypt(&(inBlock[0]), &(outBlock[0]), &aes_key);
		 memcpy(&(out[AES_BLOCK_SIZE*i]),&(outBlock[0]),AES_BLOCK_SIZE);
	 }

	 Data* result = new Data();
	 result->fastSet(out, out_length);

	 return *result;
}

Data& Crypto::AESDecode(const Data& src,const Data& key)
{
    if(src.isNull() || key.isNull()) {
        return (Data &) Data::Null;
    }

	AES_set_decrypt_key setKey = NULL;
	AES_decrypt decrypt = NULL;

	if(dlHandler == NULL) {
		openLib();
	}

	if(dlHandler) {
		setKey = (AES_set_decrypt_key)dlsym(dlHandler,"AES_set_decrypt_key");
		decrypt = (AES_decrypt)dlsym(dlHandler,"AES_decrypt");
	}

	AES_KEY aes_key;
	unsigned int out_length;

	if(setKey(key.getBytes(), key.getSize()* 8, &aes_key) < 0)
	{
	     return (Data&)Data::Null;
	}

    unsigned char *data = src.getBytes();
    unsigned int dataLength = src.getSize();

	unsigned char tmp[dataLength];
	int blockNum = dataLength/AES_BLOCK_SIZE;
	unsigned char inBlock[AES_BLOCK_SIZE+1];
	unsigned char outBlock[AES_BLOCK_SIZE];
	inBlock[AES_BLOCK_SIZE] = '\0';
	for(int i = 0; i < blockNum ; i++ ) {
		memset(&(inBlock[0]),0,AES_BLOCK_SIZE);
		memcpy(&(inBlock[0]),&(data[AES_BLOCK_SIZE*i]),AES_BLOCK_SIZE);
		decrypt(&(inBlock[0]), &(outBlock[0]), &aes_key);
		memcpy(&(tmp[AES_BLOCK_SIZE*i]),&(outBlock[0]),AES_BLOCK_SIZE);
	}

	//remove padding
	unsigned char padding = tmp[dataLength - 1];
	if(padding == 0) {
		out_length = dataLength - AES_BLOCK_SIZE;
	} else {
		out_length = dataLength - padding;
	}

	unsigned char* out = (unsigned char*)malloc(out_length + 1);
	if(out == NULL) {
		return (Data&)Data::Null;
	}

	memcpy(out,tmp,out_length);
	out[out_length] = '\0';

	Data* result = new Data();
	result->fastSet(out, out_length);

	return *result;
}

Data& Crypto::md5(const Data& data)
{
    if(data.isNull()) {
        return (Data &) Data::Null;
    }

	MD5 md5_func = NULL;

	if(dlHandler == NULL) {
		openLib();
	}

	if(dlHandler) {
		md5_func = (MD5)dlsym(dlHandler,MD5_FUNC_NAME);
	}

	unsigned char* md = (unsigned char *)malloc(sizeof(char)*17);
	memset(md,'\0',sizeof(char)*17);

	md5_func(data.getBytes(),data.getSize(),md);

	Data* result = new Data();
	result->fastSet(md, 16);

	return *result;
}

Data& Crypto::md5(FILE* fd)
{
    Data* result = new Data();
    return *result;
}

Data& Crypto::sha1(const Data& data)
{
    if(data.isNull()) {
        return (Data &) Data::Null;
    }

    SHA_CTX c;
  	SHA1 sha1_func;
    unsigned char *md = (unsigned char*)malloc(SHA_DIGEST_LENGTH);
  	if(dlHandler == NULL) {
		openLib();
	}

	if(dlHandler) {
		sha1_func = (SHA1)dlsym(dlHandler,"SHA1");
	}	
    sha1_func(data.getBytes(), data.getSize(), md);

    Data* result = new Data();
  	result->fastSet(md,SHA_DIGEST_LENGTH);

	return *result;
}  
  
Data& Crypto::sha224(const Data& data)
{
    if(data.isNull()) {
        return (Data &) Data::Null;
    }

    SHA256_CTX c;
	SHA224 sha224_func;
    unsigned char *md = (unsigned char*)malloc(SHA224_DIGEST_LENGTH);
  	if(dlHandler == NULL) {
		openLib();
	}

	if(dlHandler) {
		sha224_func = (SHA224)dlsym(dlHandler,"SHA224");
	}	
    sha224_func(data.getBytes(), data.getSize(), md);

    Data* result = new Data();
  	result->fastSet(md,SHA224_DIGEST_LENGTH);

	return *result;
}  
  
Data& Crypto::sha256(const Data& data)
{
    if(data.isNull()) {
        return (Data &) Data::Null;
    }

    SHA256_CTX c;
	SHA256 sha256_func;
    unsigned char *md = (unsigned char*)malloc(SHA256_DIGEST_LENGTH);
  	if(dlHandler == NULL) {
		openLib();
	}

	if(dlHandler) {
		sha256_func = (SHA256)dlsym(dlHandler,"SHA256");
	}	
    sha256_func(data.getBytes(), data.getSize(), md);

    Data* result = new Data();
  	result->fastSet(md,SHA256_DIGEST_LENGTH);

	return *result;
}  
  
Data& Crypto::sha384(const Data& data)
{
    if(data.isNull()) {
        return (Data &) Data::Null;
    }

    SHA512_CTX c;
	SHA384 sha384_func;
    unsigned char *md = (unsigned char*)malloc(SHA384_DIGEST_LENGTH);
  	if(dlHandler == NULL) {
		openLib();
	}

	if(dlHandler) {
		sha384_func = (SHA384)dlsym(dlHandler,"SHA384");
	}	
    sha384_func(data.getBytes(), data.getSize(), md);

    Data* result = new Data();
  	result->fastSet(md,SHA384_DIGEST_LENGTH);

	return *result;
}  
  
Data& Crypto::sha512(const Data& data)
{
    if(data.isNull()) {
        return (Data &) Data::Null;
    }

    SHA512_CTX c;
	SHA512 sha512_func;
    unsigned char *md = (unsigned char*)malloc(SHA512_DIGEST_LENGTH);
  	if(dlHandler == NULL) {
		openLib();
	}

	if(dlHandler) {
		sha512_func = (SHA512)dlsym(dlHandler,"SHA512");
	}	
    sha512_func(data.getBytes(), data.getSize(), md);

    Data* result = new Data();
  	result->fastSet(md,SHA512_DIGEST_LENGTH);

	return *result;
}  


Data& Crypto::hex2char(const Data& data)
{
    if(data.isNull()) {
        return (Data &) Data::Null;
    }

	int outLength = data.getSize()/2;
	int i;
	char tmp[2+1];
	tmp[2] = '\0';
    unsigned char* src = data.getBytes();
	unsigned char* out = (unsigned char*)malloc(outLength);
	long num;
	for(i = 0;i < outLength;i++) {
		memcpy(&(tmp[0]),&(src[i*2]),2);
		num = strtol(tmp,NULL,16);
		out[i] = num;
	}

	Data* result = new Data();
	result->fastSet(out, outLength);

	return *result;
}

Data& Crypto::char2hex(const Data& data)
{
    if(data.isNull()) {
        return (Data &) Data::Null;
    }

	int outLength = data.getSize()*2;

    unsigned char* src = data.getBytes();
	unsigned char *buf = (unsigned char *)malloc(sizeof(char)*(outLength+1));
	memset(buf,'\0',sizeof(char)*(outLength+1));
	unsigned char tmp[3]={'\0'};
	int i;

	for (i = 0; i < outLength; i++){
		  sprintf(reinterpret_cast<char*>(tmp),"%2.2x",(unsigned int)src[i]);
		  strcat(reinterpret_cast<char*>(buf),reinterpret_cast<char*>(tmp));
	}

	Data* result = new Data();
	result->fastSet(buf, outLength);
	LOG("hex encode:%s",buf);
	
	return *result;
}
