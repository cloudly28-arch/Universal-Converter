// BigFraction.cpp
#include "BigFraction.h"

// Нормализация: сокращение дроби через НОД
// Для 10 класса: работаем только с неотрицательными числами
void BigFraction::normalize() {
    if (den.isZero()) throw std::runtime_error("Denominator cannot be zero");

    // Если числитель 0, дробь = 0/1
    if (num.isZero()) {
        den = BigInteger(1);
        return;
    }

    // Сокращаем через НОД
    BigInteger g = BigInteger::gcd(num, den);
    if (!g.isZero() && !(g == BigInteger(1))) {
        auto [n, _] = num.divmod(g);  // num / g
        auto [d, __] = den.divmod(g);  // den / g
        num = n;
        den = d;
    }
}

BigInteger BigFraction::getIntegerPart() const {
    return num.divmod(den).first;
}

BigInteger BigFraction::getRemainder() const {
    return num.divmod(den).second;
}

BigFraction BigFraction::operator+(const BigFraction& other) const {
    // a/b + c/d = (a*d + c*b) / (b*d)
    BigInteger newNum = num * other.den + other.num * den;
    BigInteger newDen = den * other.den;
    return BigFraction(newNum, newDen);  // конструктор вызовет normalize()
}

BigFraction BigFraction::operator*(const BigFraction& other) const {
    // a/b * c/d = (a*c) / (b*d)
    return BigFraction(num * other.num, den * other.den);
}
