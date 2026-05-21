#pragma once

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

class BigInteger {
public:
    BigInteger() {}

    BigInteger(unsigned long long value) {
        if (value == 0) return;
        while (value > 0) {
            digits_.push_back(static_cast<int>(value % BASE));
            value /= BASE;
        }
    }

    explicit BigInteger(const std::string& value) {
        if (value.empty()) throw std::runtime_error("пустая строка для BigInteger");
        for (char c : value) {
            if (c < '0' || c > '9') throw std::runtime_error("BigInteger можно создавать только из десятичных цифр");
        }
        for (int i = static_cast<int>(value.size()); i > 0; i -= BASE_DIGITS) {
            int start = std::max(0, i - BASE_DIGITS);
            int length = i - start;
            digits_.push_back(std::stoi(value.substr(start, length)));
        }
        removeLeadingZeros();
    }
    std::string toString() const {
        if (digits_.empty()) return "0";
        std::stringstream ss;
        ss << digits_.back();
        for (int i = static_cast<int>(digits_.size()) - 2; i >= 0; --i) {
            ss << std::setw(BASE_DIGITS) << std::setfill('0') << digits_[i];
        }
        return ss.str();
    }

    bool isZero() const { return digits_.empty(); }
    BigInteger operator+(const BigInteger& other) const {
        BigInteger result;
        int carry = 0;
        size_t n = std::max(digits_.size(), other.digits_.size());
        result.digits_.reserve(n + 1);

        for (size_t i = 0; i < n || carry != 0; ++i) {
            long long cur = carry;
            if (i < digits_.size()) cur += digits_[i];
            if (i < other.digits_.size()) cur += other.digits_[i];
            result.digits_.push_back(static_cast<int>(cur % BASE));
            carry = static_cast<int>(cur / BASE);
        }
        result.removeLeadingZeros();
        return result;
    }

    BigInteger& operator+=(const BigInteger& other) {
        *this = *this + other;
        return *this;
    }
    BigInteger operator-(const BigInteger& other) const {
        if (*this < other) throw std::runtime_error("вычитание привело бы к отрицательному числу");
        BigInteger result;
        result.digits_.reserve(digits_.size());
        int carry = 0;
        for (size_t i = 0; i < digits_.size(); ++i) {
            long long cur = digits_[i] - carry;
            if (i < other.digits_.size()) cur -= other.digits_[i];
            if (cur < 0) {
                cur += BASE;
                carry = 1;
            } else {
                carry = 0;
            }
            result.digits_.push_back(static_cast<int>(cur));
        }
        result.removeLeadingZeros();
        return result;
    }

    BigInteger& operator-=(const BigInteger& other) {
        *this = *this - other;
        return *this;
    }
    BigInteger& operator*=(int value) {
        if (value < 0) throw std::runtime_error("BigInteger не поддерживает отрицательные числа");
        if (value == 0 || isZero()) { digits_.clear(); return *this; }
        long long carry = 0;
        for (size_t i = 0; i < digits_.size() || carry != 0; ++i) {
            if (i == digits_.size()) digits_.push_back(0);
            long long cur = carry + 1LL * digits_[i] * value;
            digits_[i] = static_cast<int>(cur % BASE);
            carry = cur / BASE;
        }
        removeLeadingZeros();
        return *this;
    }

    BigInteger operator*(int value) const {
        BigInteger res = *this;
        res *= value;
        return res;
    }
    BigInteger operator*(const BigInteger& other) const {
        if (isZero() || other.isZero()) return BigInteger();

        BigInteger result;
        result.digits_.assign(digits_.size() + other.digits_.size(), 0);
        for (size_t i = 0; i < digits_.size(); ++i) {
            long long carry = 0;
            for (size_t j = 0; j < other.digits_.size() || carry != 0; ++j) {
                long long cur = result.digits_[i + j] + carry;
                if (j < other.digits_.size()) {
                    cur += 1LL * digits_[i] * other.digits_[j];
                }
                result.digits_[i + j] = static_cast<int>(cur % BASE);
                carry = cur / BASE;
            }
        }
        result.removeLeadingZeros();
        return result;
    }
    BigInteger& operator*=(const BigInteger& other) {
        *this = *this * other;
        return *this;
    }
    BigInteger operator/(const BigInteger& other) const {
        if (other.isZero()) throw std::runtime_error("деление на ноль");
        if (*this < other) return BigInteger();

        BigInteger quotient;
        BigInteger remainder;
        quotient.digits_.assign(digits_.size(), 0);
        for (int i = static_cast<int>(digits_.size()) - 1; i >= 0; --i) {
            if (remainder.digits_.empty()) {
                remainder.digits_.push_back(0);
            } else {
                remainder.digits_.insert(remainder.digits_.begin(), 0);
            }
            remainder.digits_[0] = digits_[i];
            remainder.removeLeadingZeros();

            int left = 0, right = BASE - 1, digit = 0;
            while (left <= right) {
                int middle = left + (right - left) / 2;
                BigInteger current = other;
                current *= middle;
                if (current <= remainder) {
                    digit = middle;
                    left = middle + 1;
                } else {
                    right = middle - 1;
                }
            }
            quotient.digits_[i] = digit;
            remainder -= (other * digit);
        }
        quotient.removeLeadingZeros();
        return quotient;
    }

    BigInteger& operator/=(const BigInteger& other) {
        *this = *this / other;
        return *this;
    }
    BigInteger operator%(const BigInteger& other) const {
        if (other.isZero()) throw std::runtime_error("деление на ноль");
        return *this - (*this / other) * other;
    }

    BigInteger& operator%=(const BigInteger& other) {
        *this = *this % other;
        return *this;
    }
    bool operator<(const BigInteger& other) const { return compare(other) < 0; }
    bool operator>(const BigInteger& other) const { return compare(other) > 0; }
    bool operator<=(const BigInteger& other) const { return compare(other) <= 0; }
    bool operator>=(const BigInteger& other) const { return compare(other) >= 0; }
    bool operator==(const BigInteger& other) const { return compare(other) == 0; }
    bool operator!=(const BigInteger& other) const { return compare(other) != 0; }
    static BigInteger gcd(BigInteger a, BigInteger b) {
        while (!b.isZero()) {
            BigInteger r = a % b;
            a = b;
            b = r;
        }
        return a;
    }

    static BigInteger powSmall(int base, int power) {
        BigInteger result(1);
        for (int i = 0; i < power; ++i) result *= base;
        return result;
    }
    static BigInteger karatsuba(const BigInteger& a, const BigInteger& b) {
        if (a.isZero() || b.isZero()) return BigInteger();
        if (std::max(a.digits_.size(), b.digits_.size()) <= 10) {
            BigInteger res;
            res.digits_.assign(a.digits_.size() + b.digits_.size(), 0);
            for (size_t i = 0; i < a.digits_.size(); ++i) {
                long long carry = 0;
                for (size_t j = 0; j < b.digits_.size() || carry; ++j) {
                    long long cur = res.digits_[i+j] + carry + 1LL * a.digits_[i] * (j < b.digits_.size() ? b.digits_[j] : 0);
                    res.digits_[i+j] = static_cast<int>(cur % BASE);
                    carry = cur / BASE;
                }
            }
            res.removeLeadingZeros();
            return res;
        }

        size_t m = std::max(a.digits_.size(), b.digits_.size()) / 2;
        BigInteger a_low, a_high, b_low, b_high;
        size_t a_cut = std::min(a.digits_.size(), m);
        size_t b_cut = std::min(b.digits_.size(), m);

        a_low.digits_.assign(a.digits_.begin(), a.digits_.begin() + a_cut);
        b_low.digits_.assign(b.digits_.begin(), b.digits_.begin() + b_cut);
        if (a.digits_.size() > m) a_high.digits_.assign(a.digits_.begin() + m, a.digits_.end());
        if (b.digits_.size() > m) b_high.digits_.assign(b.digits_.begin() + m, b.digits_.end());

        BigInteger z0 = karatsuba(a_low, b_low);
        BigInteger z1 = karatsuba(a_high, b_high);
        BigInteger z2 = karatsuba(a_low + a_high, b_low + b_high);
        BigInteger mid = z2 - z1 - z0;
        if (m > 0) mid.digits_.insert(mid.digits_.begin(), m, 0);
        if (2 * m > 0) z1.digits_.insert(z1.digits_.begin(), 2 * m, 0);
        return z0 + mid + z1;
    }

private:
    static const int BASE = 1000000000;
    static const int BASE_DIGITS = 9;
    std::vector<int> digits_;

    void removeLeadingZeros() {
        while (!digits_.empty() && digits_.back() == 0) {
            digits_.pop_back();
        }
    }

    int compare(const BigInteger& other) const {
        if (digits_.size() != other.digits_.size()) {
            return digits_.size() < other.digits_.size() ? -1 : 1;
        }
        for (int i = static_cast<int>(digits_.size()) - 1; i >= 0; --i) {
            if (digits_[i] != other.digits_[i]) {
                return digits_[i] < other.digits_[i] ? -1 : 1;
            }
        }
        return 0;
    }
};
