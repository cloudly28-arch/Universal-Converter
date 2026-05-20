#include "baseformatter.h"

#include <algorithm>
#include <map>
#include <stdexcept>
#include <vector>

FormatResult BaseFormatter::format(const BigFraction& value, int base) {
    FormatResult result;

    if (value.isZero()) {
        result.text = "0";
        return result;
    }

    BigInteger integerPart = value.integerPart();
    BigInteger remainder = value.remainder();
    BigInteger denominator = value.denominator();

    bool truncated = false;
    addText(result.text, integerToBase(integerPart, base), truncated);

    if (remainder.isZero() || truncated) {
        result.truncated = truncated;
        return result;
    }

    addText(result.text, ".", truncated);

    std::map<BigInteger, size_t> usedRemainders;
    std::vector<std::string> digits;
    size_t periodStart = 0;
    bool hasPeriod = false;

    while (!remainder.isZero()) {
        if (usedRemainders.find(remainder) != usedRemainders.end()) {
            periodStart = usedRemainders[remainder];
            hasPeriod = true;
            break;
        }
        usedRemainders[remainder] = digits.size();

        remainder *= base;
        BigInteger digitBig = remainder / denominator;
        remainder %= denominator;

        int digit = std::stoi(digitBig.toString());
        digits.push_back(digitToString(digit));

        size_t currentLength = result.text.size();
        for (const std::string& d : digits) {
            currentLength += d.size();
        }
        if (currentLength > MAX_OUTPUT_LENGTH) {
            truncated = true;
            break;
        }
    }

    if (truncated) {
        for (const std::string& d : digits) {
            addText(result.text, d, truncated);
            if (result.text.size() >= MAX_OUTPUT_LENGTH) {
                break;
            }
        }
        addText(result.text, "...", truncated);
        result.truncated = true;
        return result;
    }

    for (size_t i = 0; i < digits.size(); ++i) {
        if (hasPeriod && i == periodStart) {
            addText(result.text, "(", truncated);
        }
        addText(result.text, digits[i], truncated);
    }
    if (hasPeriod) {
        addText(result.text, ")", truncated);
    }

    result.truncated = truncated;
    return result;
}

std::string BaseFormatter::digitToString(int digit) {
    if (digit < 0) {
        throw std::runtime_error("цифра не может быть отрицательной");
    }
    if (digit <= 9) {
        return std::string(1, static_cast<char>('0' + digit));
    }
    if (digit <= 35) {
        return std::string(1, static_cast<char>('A' + digit - 10));
    }
    return "[" + std::to_string(digit) + "]";
}

std::string BaseFormatter::integerToBase(BigInteger value, int base) {
    if (value.isZero()) {
        return "0";
    }

    std::vector<std::string> digits;
    BigInteger b(static_cast<unsigned long long>(base));
    while (!value.isZero()) {
        BigInteger digitBig = value % b;
        value /= b;
        int digit = std::stoi(digitBig.toString());
        digits.push_back(digitToString(digit));
    }

    std::reverse(digits.begin(), digits.end());
    std::string answer;
    for (const std::string& digit : digits) {
        answer += digit;
    }
    return answer;
}

bool BaseFormatter::addText(std::string& text, const std::string& add, bool& truncated) {
    if (text.size() + add.size() <= MAX_OUTPUT_LENGTH) {
        text += add;
        return true;
    }

    size_t freeSpace = MAX_OUTPUT_LENGTH > text.size() ? MAX_OUTPUT_LENGTH - text.size() : 0;
    text += add.substr(0, freeSpace);
    truncated = true;
    return false;
}
