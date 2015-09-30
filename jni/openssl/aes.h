/* crypto/aes/aes.h -*- mode:C; c-file-style: "eay" -*- */

#ifndef HEADER_AES_H
#define HEADER_AES_H

#define AES_ENCRYPT	1
#define AES_DECRYPT	0

/* Because array size can't be a const in C, the following two are macros.
   Both sizes are in bytes. */
#define AES_MAXNR 14
#define AES_BLOCK_SIZE 16

#ifdef  __cplusplus
extern "C" {
#endif

/* This should be a hidden type, but EVP requires that the size be known */
struct aes_key_st {
#ifdef AES_LONG
    unsigned long rd_key[4 *(AES_MAXNR + 1)];
#else
    unsigned int rd_key[4 *(AES_MAXNR + 1)];
#endif
    int rounds;
};
typedef struct aes_key_st AES_KEY;

typedef int (*AES_set_encrypt_key)(const unsigned char *userKey, const int bits,
	AES_KEY *key);
typedef int (*AES_set_decrypt_key)(const unsigned char *userKey, const int bits,
	AES_KEY *key);

typedef void (*AES_encrypt)(const unsigned char *in, unsigned char *out,
	const AES_KEY *key);
typedef void (*AES_decrypt)(const unsigned char *in, unsigned char *out,
	const AES_KEY *key);

#ifdef  __cplusplus
}
#endif

#endif /* !HEADER_AES_H */
