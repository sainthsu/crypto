#ifndef HEADER_CRYPTO_H
#define HEADER_CRYPTO_H

#include "Data.h"
#include <stdio.h>

class Crypto
{
public:
	static Data& hex2char(const Data& data);
	static Data& char2hex(const Data& data);

	Crypto();
    ~Crypto();
public:
	Data& AESEncode(const Data &src,const Data& key);
	Data& AESDecode(const Data &src,const Data& key);

	Data& md5(const Data &data);
    Data& md5(FILE * fd);
	//void setAESKey(const unsigned char* key);
	
	Data& sha1(const Data &data);
	Data& sha224(const Data &data);
	Data& sha256(const Data &data);
	Data& sha384(const Data &data);
	Data& sha512(const Data &data);

protected:
	unsigned char* aesKey;
	unsigned char* rsaPublicKey;
	void* dlHandler;
private:
	void openLib();
};

#endif
