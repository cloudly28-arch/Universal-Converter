// BigFraction.h
#pragma once
#include "BigInteger.h"

class BigFraction {
private:
    BigInteger num, den;  // num/den, всегда num >= 0, den > 0

    // Приватный метод нормализации (сокращение дроби)
    void normalize();

public:
    // Конструкторы
    BigFraction() : num(0), den(1) {}
    BigFraction(const BigInteger& n, const BigInteger& d) : num(n), den(d) { normalize(); }

    // Получение частей дроби
    BigInteger getIntegerPart() const;   // Целая часть: num / den
    BigInteger getRemainder() const;     // Остаток: num % den

    // Арифметика (возвращают уже нормализованные дроби)
    BigFraction operator+(const BigFraction& other) const;
    BigFraction operator*(const BigFraction& other) const;

    // Геттеры
    bool isZero() const { return num.isZero(); }
    const BigInteger& getNumerator() const { return num; }
    const BigInteger& getDenominator() const { return den; }
};
