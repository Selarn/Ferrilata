#include "math.h"
/* Warning: ancient evil !!!

#include <QMessageBox>
#include <iostream>

typedef long long int ll_int;


void BigInt::remove_leading_zeros() {
    while ( (digits.size() > 1) && (digits.back() == 0) )
        digits.pop_back();
    if ( (digits.size() < 2) && (digits[0] == 0) )
        is_negative = false;
}

BigInt &BigInt::abs_add(const BigInt &rhs) {
    size_t size = digits.size(), rhs_size = rhs.digits.size();

    if ( size == rhs_size ) {
        if ( digits[size - 1] + rhs.digits[rhs_size - 1] > BASE )
            digits.push_back( 0 );
    }
    if ( size < rhs_size ) {
        digits.resize( rhs_size );
    }
    for ( size_t i = 0; i < rhs.digits.size(); ++i ) {
        digits[i] += rhs.digits[i];
        if ( digits[i] > BASE ) {
            ++digits[i + 1];
            digits[i] -= BASE;
        }
    }
    return (*this);
}


BigInt BigInt::operator +(const BigInt &rhs) const {
    BigInt resault( *this );
    resault += rhs;
    return resault;
}

BigInt BigInt::operator -(const BigInt &rhs) const {
    BigInt resault( *this );
    resault -= rhs;
    return resault;
}

BigInt BigInt::operator *(const BigInt &rhs) const {
    BigInt resault( *this );
    resault *= rhs;
    return resault;
}

BigInt BigInt::operator /(const BigInt &rhs) const {
    BigInt resault( *this );
    resault /= rhs;
    return resault;
}

BigInt BigInt::operator %(const BigInt &rhs) const {
    BigInt resault( *this );
    resault %= rhs;
    return resault;
}

BigInt BigInt::operator ^(ll_int degree) const {
    BigInt value(1ull), resault(1ull);
    while ( degree > 0 ){
        if ( degree % 2 == 1 )
            resault *= value;
        value = value * value;
        degree /= 2;
    }
    return resault;
}


BigInt &BigInt::operator =(const BigInt &rhs) {
    is_negative = rhs.is_negative;
    digits = rhs.digits;
    return (*this);
}

BigInt &BigInt::operator +=(const BigInt &rhs) {
    if ( is_negative || rhs.is_negative ) {
        if ( !is_negative )
            return ( (*this) -= rhs );
        if ( !rhs.is_negative )
            return ( (*this) = (rhs - (*this)) ); // копирования не избежать :(
    }
    return abs_add( rhs );
}

BigInt &BigInt::operator -=(const BigInt &rhs) {
    // написано сразу после operator >
    if ( is_negative ^ rhs.is_negative )
        return abs_add( rhs );
    if ( rhs > (*this) ) {
        for ( size_t i = 0; i < digits.size(); ++i ) {
            digits[i] = rhs.digits[i] - digits[i];
            if ( digits[i] < 0 ) {
                digits[i] += BASE;
                // на след шаге результат уменьшится на 1
                ++digits[i + 1];
            }
        }
        is_negative = !is_negative;
    } else
        for ( size_t i = 0; i < rhs.digits.size(); ++i ) {
            digits[i] -= rhs.digits[i];
            if ( digits[i] < 0 ) {
                digits[i] += BASE;
                --digits[i + 1];
            }
        }
    remove_leading_zeros();
    return (*this);
}

BigInt &BigInt::operator *=(const BigInt &rhs) {
    //Karatsuba style

}

BigInt &BigInt::operator /=(const BigInt &rhs) {

}

BigInt &BigInt::operator %=(const BigInt &rhs) {

}


bool BigInt::operator ==(const BigInt &rhs) const {
    if ( is_negative != rhs.is_negative )
        return false;
    if ( digits.size() != rhs.digits.size() )
        return false;
    for ( size_t i = 0; i < digits.size(); ++i )
        if ( digits[i] != rhs.digits[i])
            return false;
    return true;
}

bool BigInt::operator !=(const BigInt& rhs) const {
    return !( (*this) == rhs );
}

bool BigInt::operator >(const BigInt &rhs) const {
    //знаю что дублирование кода это ужасно, но сейчас 4 утра
    //(или ночи) и я не могу придумать менее костыльный способ :)
    if ( is_negative ) {
        if ( !rhs.is_negative )
            return false;
        if ( digits.size() != rhs.digits.size() )
            return digits.size() < rhs.digits.size();
        for ( size_t i = digits.size() - 1; i < digits.size(); --i )
            if ( digits[i] != rhs.digits[i] )
                return digits[i] < rhs.digits[i];
    } else {
        if ( rhs.is_negative )
            return true;
        if ( digits.size() != rhs.digits.size() )
            return digits.size() > rhs.digits.size();
        for ( size_t i = digits.size() - 1; i < digits.size(); --i )
            if ( digits[i] != rhs.digits[i] )
                return digits[i] > rhs.digits[i];
    }
    return false;
}


QDataStream &operator >>(QDataStream &in, BigInt &rhs) {
    // Я не буду пытаться реализовывать RSA на potato computer
    if ( sizeof(std::size_t) < 4 ) {
        QMessageBox::critical( for_alerts, "Your disk will be formatted (no) :)",
                               "This app can't work on your potato. Increase the amount of RAM");
        throw("Let me die");
    }
    int32_t size;
    in >> size;
    rhs.digits.reserve( size );
    //for ( int32_t i = size - 1; i >= 0; --i )
        //in >> rhs.digits[i];
    //in >> rhs.is_negative;
    return in;
}

QDataStream &operator <<(QDataStream &out, const BigInt &rhs) {
    int32_t size = rhs.digits.size();
    out << size;
    //for ( int32_t i = 0; i < size; ++i )
        //out << rhs.digits[i];
    //out << rhs.is_negative;
    return out;
}


BigInt::BigInt(const BigInt &rhs) {
    (*this) = rhs;
}

BigInt::BigInt(unsigned long long n) {
    is_negative = false;
    do {
        digits.push_back( n % BASE );
        n /= BASE;
    } while ( n > 0 );
}

BigInt::BigInt(long long n) : BigInt( (unsigned long long)( n < 0 ? -n : n ) ) {
    if ( n < 0 )
        is_negative = true;
}

BigInt::~BigInt() { }
*/
