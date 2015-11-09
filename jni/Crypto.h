#ifndef HEADER_CRYPTO_H
#define HEADER_CRYPTO_H

#include "Data.h"

class Crypto
{
public:
	static Data* hex2char(Data& data);
	static Data* char2hex(Data& data);

	Crypto();
    ~Crypto();

	Data* AESEncode(Data &src,Data& key);
	Data* AESDecode(Data &src,Data& key);

	Data* md5(Data &data);
    Data* md5(FILE * fd);
	//void setAESKey(const unsigned char* key);
	
	Data* sha1(Data &data);
	Data* sha224(Data &data);
	Data* sha256(Data &data);
	Data* sha384(Data &data);
	Data* sha512(Data &data);

protected:
	unsigned char* aesKey;
	unsigned char* rsaPublicKey;
	void* dlHandler;
private:
	void openLib();
};

#endif
