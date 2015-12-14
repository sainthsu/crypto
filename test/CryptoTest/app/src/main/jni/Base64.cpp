#include "Base64.h"
#include "Data.h"

#include <string.h>
#include <stdint.h>

static const char* b64_alphabet =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/=";

static const unsigned char map[123] = {  
255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
253, 255, 255, 253, 255, 255, 255, 255, 255, 255,
255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
255, 255, 253, 255, 255, 255, 255, 255, 255, 255,
255, 255, 255,  62, 255, 255, 255,  63,  52,  53,
 54,  55,  56,  57,  58,  59,  60,  61, 255, 255,  
255, 254, 255, 255, 255,   0,   1,   2,   3,   4,
  5,   6,   7,   8,   9,  10,  11,  12,  13,  14,
  15,  16,  17,  18, 19,  20,  21,  22,  23,  24,
 25, 255, 255, 255, 255, 255, 255,  26,  27,  28,
 29,  30,  31,  32,  33,  34,  35,  36,  37,  38,
 39,  40,  41,  42,  43,  44,  45,  46,  47,  48,  
 49,  50,  51 };

static char findPos(char ch)
{
	char *ptr = strrchr(b64_alphabet, ch);//the last position (the only) in b64_alphabet[]
    return (ptr - b64_alphabet); 
}

Data& base64_encode(const Data &data)
{
    if(data.isNull()) {
        return (Data &) Data::Null;
    }
 /* The tricky thing about this is doing the padding at the end,
   * doing the bit manipulation requires a bit of concentration only */
  char *buffer = NULL;
  char *point = NULL;
  int inlen = 0;
  int outlen = 0;

  char* text = (char *)data.getBytes();
  /* check data bytes */
  if (text == NULL)
    return (Data &) Data::Null;
  
  /* Use 'buffer' to store the output. Work out how big it should be...
   * This must be a multiple of 4 bytes */
  
  inlen = data.getSize();
  /* check our arg...avoid a pesky FPE */
  if (inlen == 0)
  {
      return (Data &) Data::Null;
  }
  outlen = (inlen*4)/3;
  if( (inlen % 3) > 0 ) /* got to pad */
    outlen += 4 - (inlen % 3);
  
  buffer = (char*)malloc( outlen + 1 ); /* +1 for the \0 */
  memset(buffer, 0, outlen + 1); /* initialize to zero */
  
  /* now do the main stage of conversion, 3 bytes at a time,
   * leave the trailing bytes (if there are any) for later */
  
  for( point=buffer; inlen>=3; inlen-=3, text+=3 ) {
    *(point++) = b64_alphabet[ *text>>2 ]; 
    *(point++) = b64_alphabet[ (*text<<4 & 0x30) | *(text+1)>>4 ]; 
    *(point++) = b64_alphabet[ (*(text+1)<<2 & 0x3c) | *(text+2)>>6 ];
    *(point++) = b64_alphabet[ *(text+2) & 0x3f ];
  }
  
  /* Now deal with the trailing bytes */
  if( inlen ) {
    /* We always have one trailing byte */
    *(point++) = b64_alphabet[ *text>>2 ];
    *(point++) = b64_alphabet[ (*text<<4 & 0x30) |
			     (inlen==2?*(text+1)>>4:0) ]; 
    *(point++) = (inlen==1?'=':b64_alphabet[ *(text+1)<<2 & 0x3c ] );
    *(point++) = '=';
  }
  
  *point = '\0';

  Data* result = new Data();
  result->fastSet((unsigned char*)buffer,outlen);
  
  return *result;
}

Data& base64_decode(const Data &data)
{
    if(data.isNull()) {
        return (Data &) Data::Null;
    }

  char *buffer = NULL;
  char *point = NULL;
  int inlen = 0;
  int outlen = 0;
  int group = 3;

  int32_t tmp = 0;
  char tmpCh = 0;

  char* text = (char*)data.getBytes();
  /* check data bytes */
  if (text == NULL)
    return (Data &) Data::Null;

  inlen = data.getSize();
  /* check our arg...avoid a pesky FPE */
  if (inlen == 0)
  {
      return (Data &) Data::Null;
  }
  outlen = (inlen * 3)/4;
  if( (inlen % 4) > 0 ) /* error size */
    return (Data &) Data::Null;

  int i = 1;
  while(i < 4) {
  	if(text[inlen-i] != '=') {
		break;
	}
	i++;
  }
  outlen -= (i-1); 
  buffer = (char*)malloc( outlen + 1 ); /* +1 for the \0 */
  memset(buffer, 0, outlen + 1); /* initialize to zero */
  
  /* now do the main stage of conversion, 4 bytes at a time */
  i = 0;
  int y = 0;
  int z = 0;
  char c = 0;
  for( point=buffer; i < inlen; i++) {
	    c = text[i];
		if(c > 122) return (Data &) Data::Null; //error char bigger than 'z'

		tmpCh = map[c];	
        if (tmpCh == 255) return (Data &) Data::Null;
        if (tmpCh == 253) continue; //'\n' and '\r'
        if (tmpCh == 254) { tmpCh = 0; group--; }  //'='
        tmp = (tmp << 6) | tmpCh;  
        if (++y == 4) { 
            buffer[z++] = (unsigned char)((tmp >> 16)&255);  
            if (group > 1) buffer[z++] = (unsigned char)((tmp >> 8)&255);
            if (group > 2) buffer[z++] = (unsigned char)(tmp&255);
            y = tmp = 0;  
        }
  }
  buffer[outlen] = '\0';

  Data* result = new Data();
  result->fastSet((unsigned char*)buffer,outlen);
  
  return *result;
}

