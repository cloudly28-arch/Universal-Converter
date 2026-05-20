#pragma once

#include "BigInteger.h"

class BigFraction {
public:
    BigFraction();
    explicit BigFraction(const BigInteger& numerator);
    BigFraction(const BigInteger& numerator, const BigInteger& denominator);

    const BigInteger& numerator() const;
    const BigInteger& denominator() const;

    BigInteger integerPart() const;
    BigInteger remainder() const;
    bool isZero() const;

    BigFraction& operator+=(const BigFraction& other);
    BigFraction& operator-=(const BigFraction& other);
    BigFraction& operator*=(const BigFraction& other);
    BigFraction& operator/=(const BigFraction& other);

    friend BigFraction operator+(BigFraction left, const BigFraction& right);
    friend BigFraction operator-(BigFraction left, const BigFraction& right);
    friend BigFraction operator*(BigFraction left, const BigFraction& right);
    friend BigFraction operator/(BigFraction left, const BigFraction& right);

private:
    BigInteger numerator_;
    BigInteger denominator_;

    void reduce();
};
