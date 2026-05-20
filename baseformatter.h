#pragma once

#include "BigFraction.h"

#include <string>

struct FormatResult {
    std::string text;
    bool truncated = false;
};

class BaseFormatter {
public:
    static const size_t MAX_OUTPUT_LENGTH = 100000;

    static FormatResult format(const BigFraction& value, int base);

private:
    static std::string digitToString(int digit);
    static std::string integerToBase(BigInteger value, int base);
    static bool addText(std::string& text, const std::string& add, bool& truncated);
};
