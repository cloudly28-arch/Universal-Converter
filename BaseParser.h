// BaseParser.h
#pragma once
#include "BigFraction.h"
#include <string>

class BaseParser {
public:
    // Разбирает строку вида "123.45(67)" в системе счисления base
    static BigFraction parse(const std::string& input, int base);

private:
    // Преобразует строку цифры (напр. "A", "[222]") в значение
    static int parseDigit(const std::string& token, int base);
    // Преобразует строку в BigInteger в заданной системе счисления
    static BigInteger parseNumber(const std::string& str, int base);
};
