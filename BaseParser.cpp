// BaseParser.cpp (ключевая логика)
#include "BaseParser.h"
#include <cctype>
#include <stdexcept>

int BaseParser::parseDigit(const std::string& token, int base) {
    if (token.empty()) throw std::runtime_error("Empty digit");

    if (token[0] == '[') {
        // [222]
        if (token.back() != ']') throw std::runtime_error("Unclosed bracket");
        std::string num = token.substr(1, token.size() - 2);
        int val = std::stoi(num);
        if (val >= base) throw std::runtime_error("Digit " + token + " >= base " + std::to_string(base));
        return val;
    }

    // Одиночный символ: 0-9, A-Z
    char c = std::toupper(token[0]);
    int val;
    if (c >= '0' && c <= '9') val = c - '0';
    else if (c >= 'A' && c <= 'Z') val = c - 'A' + 10;
    else throw std::runtime_error("Invalid digit: " + token);

    if (val >= base) throw std::runtime_error("Digit " + token + " >= base " + std::to_string(base));
    return val;
}

BigInteger BaseParser::parseNumber(const std::string& str, int base) {
    BigInteger result(0);
    for (char c : str) {
        int digit;
        if (c >= '0' && c <= '9') digit = c - '0';
        else if (c >= 'A' && c <= 'Z') digit = c - 'A' + 10;
        else if (c >= 'a' && c <= 'z') digit = c - 'a' + 10;
        else continue;  // игнорируем (на практике — ошибка)

        if (digit >= base) throw std::runtime_error("Invalid digit for base " + std::to_string(base));
        result = result * base + digit;
    }
    return result;
}

BigFraction BaseParser::parse(const std::string& input, int base) {
    // Упрощённый разбор: только целые и конечные дроби
    // Периодические — опционально для продвинутых

    size_t dotPos = input.find('.');
    size_t parenPos = input.find('(');

    std::string intPart = (dotPos == std::string::npos) ? input : input.substr(0, dotPos);
    BigInteger integer = intPart.empty() ? BigInteger(0) : parseNumber(intPart, base);

    if (dotPos == std::string::npos) {
        return BigFraction(integer, BigInteger(1));
    }

    // Дробная часть
    std::string fracPart;
    if (parenPos != std::string::npos) {
        fracPart = input.substr(dotPos + 1, parenPos - dotPos - 1);
        // Период: упрощённо не обрабатываем
    } else {
        fracPart = input.substr(dotPos + 1);
    }

    // x = integer + frac / base^k
    BigInteger fracVal = fracPart.empty() ? BigInteger(0) : parseNumber(fracPart, base);
    BigInteger basePower(1);
    for (size_t i = 0; i < fracPart.size(); ++i)
        basePower = basePower * base;

    BigInteger numerator = integer * basePower + fracVal;
    return BigFraction(numerator, basePower);
}
