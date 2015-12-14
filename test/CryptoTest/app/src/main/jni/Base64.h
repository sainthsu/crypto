#ifndef HEADER_BASE64_H
#define HEADER_BASE64_H

class Data;

Data& base64_encode(const Data &data);
Data& base64_decode(const Data &data);

#endif
