#ifndef CRYPTO_H
#define CRYPTO_H

#include "Data.h"

class Crypto
{
public:
	Crypto();
    ~Crypto();

	Data* AESEncode(Data &src,Data& key);
	Data* AESDecode(Data &src,Data& key);

	Data* md5(Data &data);
	//void setAESKey(const unsigned char* key);
protected:
	unsigned char* aesKey;
	unsigned char* rsaPublicKey;
	void* dlHandler;
private:
	void openLib();
};

#endif
