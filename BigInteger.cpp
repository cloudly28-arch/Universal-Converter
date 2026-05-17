// BigInteger.cpp
#include "BigInteger.h"
#include <cctype>
#include <algorithm>

void BigInteger::removeLeadingZeros() {
    while (digits.size() > 1 && digits.back() == 0)
        digits.pop_back();
    if (digits.empty()) digits.push_back(0);
}

BigInteger::BigInteger(long long num) {
    if (num == 0) digits = {0};
    else {
        while (num > 0) {
            digits.push_back(num % BASE);
            num /= BASE;
        }
    }
}

BigInteger::BigInteger(const std::string& str) {
    digits.clear();
    std::string s = str;
    // Удаляем лидирующие нули
    size_t start = 0;
    while (start < s.size() - 1 && s[start] == '0') start++;
    s = s.substr(start);

    // Разбиваем строку на блоки по 9 цифр (снизу вверх)
    for (int i = (int)s.size(); i > 0; i -= BASE_DIGITS) {
        int x = 0;
        int from = std::max(0, i - BASE_DIGITS);
        for (int j = from; j < i; ++j)
            if (std::isdigit(s[j])) x = x * 10 + (s[j] - '0');
        digits.push_back(x);
    }
    if (digits.empty()) digits.push_back(0);
    removeLeadingZeros();
}

std::string BigInteger::toString() const {
    if (isZero()) return "0";
    std::string res;
    for (int i = (int)digits.size() - 1; i >= 0; --i) {
        std::string part = std::to_string(digits[i]);
        if (i < (int)digits.size() - 1)
            part = std::string(BASE_DIGITS - part.size(), '0') + part;
        res += part;
    }
    return res;
}

bool BigInteger::operator<(const BigInteger& other) const {
    if (digits.size() != other.digits.size())
        return digits.size() < other.digits.size();
    for (int i = (int)digits.size() - 1; i >= 0; --i)
        if (digits[i] != other.digits[i])
            return digits[i] < other.digits[i];
    return false;
}

bool BigInteger::operator==(const BigInteger& other) const {
    return digits == other.digits;
}

BigInteger BigInteger::operator+(const BigInteger& other) const {
    BigInteger res;
    res.digits.clear();
    int carry = 0;
    size_t n = std::max(digits.size(), other.digits.size());
    for (size_t i = 0; i < n || carry; ++i) {
        long long cur = carry;
        if (i < digits.size()) cur += digits[i];
        if (i < other.digits.size()) cur += other.digits[i];
        res.digits.push_back(cur % BASE);
        carry = cur / BASE;
    }
    return res;
}

BigInteger BigInteger::operator-(const BigInteger& other) const {
    BigInteger res = *this;
    int borrow = 0;
    for (size_t i = 0; i < other.digits.size() || borrow; ++i) {
        long long cur = res.digits[i] - borrow;
        if (i < other.digits.size()) cur -= other.digits[i];
        if (cur < 0) { cur += BASE; borrow = 1; }
        else borrow = 0;
        res.digits[i] = cur;
    }
    res.removeLeadingZeros();
    return res;
}

BigInteger BigInteger::operator*(int small) const {
    if (small == 0) return BigInteger(0);
    BigInteger res;
    res.digits.clear();
    long long carry = 0;
    for (size_t i = 0; i < digits.size() || carry; ++i) {
        if (i < digits.size()) carry += (long long)digits[i] * small;
        res.digits.push_back(carry % BASE);
        carry /= BASE;
    }
    res.removeLeadingZeros();
    return res;
}

BigInteger BigInteger::operator/(int small) const {
    BigInteger res;
    res.digits.resize(digits.size());
    long long rem = 0;
    for (int i = (int)digits.size() - 1; i >= 0; --i) {
        long long cur = digits[i] + rem * BASE;
        res.digits[i] = cur / small;
        rem = cur % small;
    }
    res.removeLeadingZeros();
    return res;
}

int BigInteger::operator%(int small) const {
    long long rem = 0;
    for (int i = (int)digits.size() - 1; i >= 0; --i)
        rem = (rem * BASE + digits[i]) % small;
    return (int)rem;
}

// Школьное умножение O(N²). Для 10 класса и оснований до 500 работает мгновенно.
BigInteger BigInteger::operator*(const BigInteger& other) const {
    if (isZero() || other.isZero()) return BigInteger(0);

    BigInteger res;
    res.digits.assign(digits.size() + other.digits.size(), 0);

    for (size_t i = 0; i < digits.size(); ++i) {
        long long carry = 0;
        for (size_t j = 0; j < other.digits.size() || carry; ++j) {
            long long cur = res.digits[i + j] +
                            (long long)digits[i] * (j < other.digits.size() ? other.digits[j] : 0) +
                            carry;
            res.digits[i + j] = cur % BASE;
            carry = cur / BASE;
        }
    }
    res.removeLeadingZeros();
    return res;
}

// Деление с остатком. Использует бинарный поиск для цифры частного, чтобы не зависнуть.
std::pair<BigInteger, BigInteger> BigInteger::divmod(const BigInteger& other) const {
    if (other.isZero()) throw std::runtime_error("Division by zero");
    if (*this < other) return {BigInteger(0), *this};

    BigInteger quotient(0);
    BigInteger remainder(0);

    for (int i = (int)digits.size() - 1; i >= 0; --i) {
        remainder = remainder * BASE + digits[i];

        // Ищем максимальное q: other * q <= remainder
        int low = 0, high = BASE - 1;
        int q = 0;
        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (!(remainder < other * mid)) {
                q = mid;
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }

        quotient = quotient * BASE + q;
        remainder = remainder - other * q;
    }
    return {quotient, remainder};
}

BigInteger BigInteger::gcd(BigInteger a, BigInteger b) {
    while (!b.isZero()) {
        BigInteger r = a.divmod(b).second;
        a = b;
        b = r;
    }
    return a;
}
