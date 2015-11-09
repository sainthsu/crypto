/**
 * author Steve Hsu
 * steve@kunkua.com
 */
#include "Crypto.h"
#include "Config.h"

#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include <dlfcn.h>

#include "openssl/aes.h"
#include "openssl/md5.h"
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
		LOGE("dlopen Error:%s",dlerror());
}

/**
 * all char array must end of '\0'
 */
Data* Crypto::AESEncode(Data& data,Data& key)
{
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

	 if(setKey(key._bytes, key._size*8, &aes_key) < 0)
	 {
	      return NULL;
	 }

	 unsigned int data_length = data._size;
	 unsigned int out_length = data_length;

	 //add PKCS7 Padding
	 //If the original data is a multiple of N bytes, then an extra block of bytes with value N is added
	 //http://stackoverflow.com/questions/3283787/size-of-data-after-aes-cbc-and-aes-ecb-encryption
	 int padding = 0;
	 unsigned int i = 0;
	 if (data_length % AES_BLOCK_SIZE > 0)
	 {
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
	 memcpy(tmp,data,data_length);
	 tmp[out_length] = '\0';

	 while (i > 0)
	 {
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

	 return result;
}

Data* Crypto::AESDecode(Data& data,Data& key)
{
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

	if(setKey(key._bytes, key._size* 8, &aes_key) < 0)
	{
	     return NULL;
	}

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
		return NULL;
	}

	memcpy(out,tmp,out_length);
	out[out_length] = '\0';

	Data* result = new Data();
	result->fastSet(out, out_length);

	return result;
}

Data* Crypto::md5(Data& data)
{
	MD5 md5_func = NULL;

	if(dlHandler == NULL) {
		openLib();
	}

	if(dlHandler) {
		md5_func = (MD5)dlsym(dlHandler,MD5_FUNC_NAME);
	}

	unsigned char* md = (unsigned char *)malloc(sizeof(char)*17);
	memset(md,'\0',sizeof(char)*17);

	md5_func(data,strlen(reinterpret_cast<const char*>(data)),md);

	Data* result = new Data();
	result->fastSet(md, 16);

	return result;
}

Data* Crypto::md5(FILE* fd)
{
    Data* result = new Data();
    return result;
}

Data* Crypto::sha1(Data& data)  
{  
    SHA_CTX c;  
    unsigned char md[SHA_DIGEST_LENGTH];  
    SHA1((unsigned char *)orgStr, strlen(orgStr), md);  
    printHash(md, SHA_DIGEST_LENGTH);  
  
    SHA1_Init(&c);  
    SHA1_Update(&c, orgStr, strlen(orgStr));  
    SHA1_Final(md, &c);  
    OPENSSL_cleanse(&c, sizeof(c));  
    printHash(md, SHA_DIGEST_LENGTH);  
}  
  
Data* Crypto::sha224(Data& data)  
{  
    SHA256_CTX c;  
    unsigned char md[SHA224_DIGEST_LENGTH];  
    SHA224((unsigned char *)orgStr, strlen(orgStr), md);  
    printHash(md, SHA224_DIGEST_LENGTH);  
  
    SHA224_Init(&c);  
    SHA224_Update(&c, orgStr, strlen(orgStr));  
    SHA224_Final(md, &c);  
    OPENSSL_cleanse(&c, sizeof(c));  
    printHash(md, SHA224_DIGEST_LENGTH);  
}  
  
Data* Crypto::sha256(Data& data)  
{  
    SHA256_CTX c;  
    unsigned char md[SHA256_DIGEST_LENGTH];  
    SHA256((unsigned char *)orgStr, strlen(orgStr), md);  
    printHash(md, SHA256_DIGEST_LENGTH);  
  
    SHA256_Init(&c);  
    SHA256_Update(&c, orgStr, strlen(orgStr));  
    SHA256_Final(md, &c);  
    OPENSSL_cleanse(&c, sizeof(c));  
    printHash(md, SHA256_DIGEST_LENGTH);  
}  
  
Data* Crypto::sha384(Data& data)  
{  
    SHA512_CTX c;  
    unsigned char md[SHA384_DIGEST_LENGTH];  
    SHA384((unsigned char *)orgStr, strlen(orgStr), md);  
    printHash(md, SHA384_DIGEST_LENGTH);  
  
    SHA384_Init(&c);  
    SHA384_Update(&c, orgStr, strlen(orgStr));  
    SHA384_Final(md, &c);  
    OPENSSL_cleanse(&c, sizeof(c));  
    printHash(md, SHA384_DIGEST_LENGTH);  
}  
  
Data* Crypto::shah512(Data& data)  
{  
    SHA512_CTX c;  
    unsigned char md[SHA512_DIGEST_LENGTH];  
    SHA512((unsigned char *)orgStr, strlen(orgStr), md);  
    printHash(md, SHA512_DIGEST_LENGTH);  
  
    SHA512_Init(&c);  
    SHA512_Update(&c, orgStr, strlen(orgStr));  
    SHA512_Final(md, &c);  
    OPENSSL_cleanse(&c, sizeof(c));  
    printHash(md, SHA512_DIGEST_LENGTH);  
}  
  



Data* Crypto::hex2char(Data& data)
{
	int outLength = data._size/2;
	int i;
	char tmp[2+1];
	tmp[2] = '\0';
	unsigned char* out = (unsigned char*)malloc(outLength);
	long num;
	for(i = 0;i < outLength;i++) {
		memcpy(&(tmp[0]),&(data._bytes[i*2]),2);
		num = strtol(tmp,NULL,16);
		out[i] = num;
	}

	Data* result = new Data();
	result->fastSet(out, _outLength);

	return result;
}

Data* Crypto::char2hex(Data& data)
{
	int outLength = data._size*2;

	unsigned char *buf = (unsigned char *)malloc(sizeof(char)*(outLength+1));
	memset(buf,'\0',sizeof(char)*(outLength+1));
	unsigned char tmp[3]={'\0'};
	int i;

	for (i = 0; i < length; i++){
		  sprintf(reinterpret_cast<char*>(tmp),"%2.2x",(unsigned int)data._bytes[i]);
		  strcat(reinterpret_cast<char*>(buf),reinterpret_cast<char*>(tmp));
	}

	Data* result = new Data();
	result->fastSet(buf, outLength);
	LOG("hex encode:%s",buf);
	
	return result;
}
