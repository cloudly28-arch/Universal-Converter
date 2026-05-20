#pragma once

#include "BigFraction.h"

#include <string>
#include <vector>

class BaseParser {
public:
    static BigFraction parse(const std::string& text, int base);

private:
    struct ParsedNumber {
        std::vector<int> integerDigits;
        std::vector<int> fractionDigits;
        std::vector<int> periodDigits;
        bool hasPoint = false;
        bool hasPeriod = false;
    };

    static ParsedNumber readDigits(const std::string& text, int base);
    static int readOneDigit(const std::string& text, int& pos, int base);
    static BigInteger digitsToInteger(const std::vector<int>& digits, int base);
};
