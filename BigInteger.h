// BigInteger.h
#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>

class BigInteger {
private:
    static const int BASE = 1000000000;      // 10^9
    static const int BASE_DIGITS = 9;
    std::vector<int> digits;                 // little-endian: [младшие, ..., старшие]

    // Вспомогательный метод (private)
    void removeLeadingZeros();

public:
    // Конструкторы
    BigInteger() : digits(1, 0) {}
    BigInteger(long long num);
    BigInteger(const std::string& str);

    // Преобразование в строку
    std::string toString() const;

    // Проверка на ноль (ЕДИНСТВЕННОЕ объявление!)
    bool isZero() const {
        return digits.empty() || (digits.size() == 1 && digits[0] == 0);
    }

    // Сравнения
    bool operator<(const BigInteger& other) const;
    bool operator==(const BigInteger& other) const;
    bool operator<=(const BigInteger& other) const { return *this < other || *this == other; }
    bool operator>(const BigInteger& other) const { return !(*this <= other); }
    bool operator>=(const BigInteger& other) const { return !(*this < other); }
    bool operator!=(const BigInteger& other) const { return !(*this == other); }

    // Арифметика
    BigInteger operator+(const BigInteger& other) const;
    BigInteger operator-(const BigInteger& other) const;  // только если *this >= other
    BigInteger operator*(const BigInteger& other) const;
    BigInteger operator*(int small) const;   // умножение на маленькое число
    BigInteger operator/(int small) const;   // деление на маленькое число
    int operator%(int small) const;          // остаток от деления на маленькое

    // Деление с остатком (возвращает пару: частное, остаток)
    std::pair<BigInteger, BigInteger> divmod(const BigInteger& other) const;

    // НОД (алгоритм Евклида)
    static BigInteger gcd(BigInteger a, BigInteger b);

    // Доступ к внутренним данным (для отладки)
    const std::vector<int>& getDigits() const { return digits; }
};
