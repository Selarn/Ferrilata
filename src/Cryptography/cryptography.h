#ifndef CRYPTOGRAPHY_H
#define CRYPTOGRAPHY_H


#include "math.h"

#include <QString>

typedef long long int ll_int;

uint64_t get_random_number();

class RSA {
    BigInt public_key;
    BigInt private_key;
public:
    static std::pair<BigInt, BigInt> encrypt( BigInt* data, BigInt* key );
    RSA( ll_int a = 0, ll_int b = 0 );
    ~RSA();
};


void DAES_encrypt( BigInt* key, QString& text );


#endif // CRYPTOGRAPHY_H
