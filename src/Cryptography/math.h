#ifndef MATH_H
#define MATH_H

#include <vector>
#include <cstdint>
#include <QWidget>

extern QWidget* for_alerts;

class BigInt {

        static const int32_t BASE = 1000000000;
        std::vector<int64_t> digits;
        int8_t is_negative;

        void remove_leading_zeros();

        BigInt& abs_add( const BigInt& rhs );
        // |*this| += |rhs| without copy

        BigInt& sub( const BigInt& rhs );
        // *this -= |rhs| without copy )

    public:

        BigInt operator + ( const BigInt& rhs ) const;
        BigInt operator - ( const BigInt& rhs ) const;
        BigInt operator * ( const BigInt& rhs ) const;
        BigInt operator / ( const BigInt& rhs ) const;
        BigInt operator % ( const BigInt& rhs ) const;
        BigInt operator ^ ( long long int degree ) const;

        BigInt& operator = ( const BigInt& rhs );
        BigInt& operator += ( const BigInt& rhs );
        BigInt& operator -= ( const BigInt& rhs ); // оптимизировать
        BigInt& operator *= ( const BigInt& rhs ); // оптимизировать
        BigInt& operator /= ( const BigInt& rhs );
        BigInt& operator %= ( const BigInt& rhs );

        bool operator == ( const BigInt& rhs ) const;
        bool operator != ( const BigInt& rhs ) const;
        bool operator >  ( const BigInt& rhs ) const; // оптимизировать
        bool operator <  ( const BigInt& rhs ) = delete; // реализовать
        bool operator >= ( const BigInt& rhs ) = delete; // реализовать
        bool operator <= ( const BigInt& rhs ) = delete; // реализовать

        friend QDataStream& operator >> ( QDataStream& in, BigInt& rhs );
        friend QDataStream& operator << ( QDataStream& out, const BigInt& rhs );

        BigInt( const BigInt& rhs );
        BigInt( unsigned long long int n );
        BigInt( long long int n );
        ~BigInt();
};

class Part {
    BigInt number;
    size_t begin;
    size_t end;
public:
    Part( BigInt& num, size_t beg, size_t end );
};

#endif // MATH_H
