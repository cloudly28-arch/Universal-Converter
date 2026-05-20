#include "BigFraction.h"

#include <stdexcept>

BigFraction::BigFraction() : numerator_(0), denominator_(1) {
}

BigFraction::BigFraction(const BigInteger& numerator)
    : numerator_(numerator), denominator_(1) {
}

BigFraction::BigFraction(const BigInteger& numerator, const BigInteger& denominator)
    : numerator_(numerator), denominator_(denominator) {
    if (denominator_.isZero()) {
        throw std::runtime_error("знаменатель дроби не может быть равен нулю");
    }
    reduce();
}

const BigInteger& BigFraction::numerator() const {
    return numerator_;
}

const BigInteger& BigFraction::denominator() const {
    return denominator_;
}

BigInteger BigFraction::integerPart() const {
    return numerator_ / denominator_;
}

BigInteger BigFraction::remainder() const {
    return numerator_ % denominator_;
}

bool BigFraction::isZero() const {
    return numerator_.isZero();
}

void BigFraction::reduce() {
    if (numerator_.isZero()) {
        denominator_ = BigInteger(1);
        return;
    }
    BigInteger d = BigInteger::gcd(numerator_, denominator_);
    numerator_ /= d;
    denominator_ /= d;
}

BigFraction& BigFraction::operator+=(const BigFraction& other) {
    numerator_ = numerator_ * other.denominator_ + other.numerator_ * denominator_;
    denominator_ = denominator_ * other.denominator_;
    reduce();
    return *this;
}

BigFraction& BigFraction::operator-=(const BigFraction& other) {
    BigInteger left = numerator_ * other.denominator_;
    BigInteger right = other.numerator_ * denominator_;
    if (left < right) {
        throw std::runtime_error("результат вычитания дробей отрицательный");
    }
    numerator_ = left - right;
    denominator_ = denominator_ * other.denominator_;
    reduce();
    return *this;
}

BigFraction& BigFraction::operator*=(const BigFraction& other) {
    numerator_ = numerator_ * other.numerator_;
    denominator_ = denominator_ * other.denominator_;
    reduce();
    return *this;
}

BigFraction& BigFraction::operator/=(const BigFraction& other) {
    if (other.numerator_.isZero()) {
        throw std::runtime_error("деление дроби на ноль");
    }
    numerator_ = numerator_ * other.denominator_;
    denominator_ = denominator_ * other.numerator_;
    reduce();
    return *this;
}

BigFraction operator+(BigFraction left, const BigFraction& right) {
    left += right;
    return left;
}

BigFraction operator-(BigFraction left, const BigFraction& right) {
    left -= right;
    return left;
}

BigFraction operator*(BigFraction left, const BigFraction& right) {
    left *= right;
    return left;
}

BigFraction operator/(BigFraction left, const BigFraction& right) {
    left /= right;
    return left;
}
