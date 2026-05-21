#include "BaseParser.h"

#include <cctype>
#include <stdexcept>

BigFraction BaseParser::parse(const std::string& text, int base) {
    ParsedNumber parsed = readDigits(text, base);

    BigInteger integerValue = digitsToInteger(parsed.integerDigits, base);
    BigInteger fractionValue = digitsToInteger(parsed.fractionDigits, base);
    BigInteger periodValue = digitsToInteger(parsed.periodDigits, base);

    int fractionSize = static_cast<int>(parsed.fractionDigits.size());
    int periodSize = static_cast<int>(parsed.periodDigits.size());

    BigInteger baseInFractionPower = BigInteger::powSmall(base, fractionSize);

    if (!parsed.hasPeriod) {
        BigInteger numerator = integerValue * baseInFractionPower + fractionValue;
        BigInteger denominator = baseInFractionPower;

        return BigFraction(numerator, denominator);
    }

    BigInteger baseInPeriodPower = BigInteger::powSmall(base, periodSize);
    BigInteger periodPart = baseInPeriodPower - BigInteger(1);

    BigInteger denominator = baseInFractionPower * periodPart;

    BigInteger numerator = integerValue * denominator
                           + fractionValue * periodPart
                           + periodValue;

    return BigFraction(numerator, denominator);
}

BaseParser::ParsedNumber BaseParser::readDigits(const std::string& text, int base) {
    if (text.empty()) {
        throw std::runtime_error("Ошибка: входная строка пуста.");
    }

    ParsedNumber result;

    int pos = 0;
    int part = 0;

    bool periodClosed = false;
    while (pos < static_cast<int>(text.size())) {
        char ch = text[pos];
        if (std::isspace(static_cast<unsigned char>(ch))) {
            throw std::runtime_error("Ошибка: пробелы внутри числа не допускаются.");
        }

        if (periodClosed) {
            throw std::runtime_error("Ошибка: после закрывающей скобки периода не должно быть других символов.");
        }

        if (ch == '.') {
            if (result.hasPoint) {
                throw std::runtime_error("Ошибка: точка встречается больше одного раза.");
            }

            if (result.integerDigits.empty()) {
                throw std::runtime_error("Ошибка: отсутствует целая часть до точки.");
            }

            result.hasPoint = true;
            part = 1;
            ++pos;
            continue;
        }

        if (ch == '(') {
            if (!result.hasPoint) {
                throw std::runtime_error("Ошибка: период может быть только после дробной точки.");
            }

            if (result.hasPeriod) {
                throw std::runtime_error("Ошибка: период указан больше одного раза.");
            }
            result.hasPeriod = true;
            part = 2;
            ++pos;
            continue;
        }
        if (ch == ')') {
            if (!result.hasPeriod || part != 2) {
                throw std::runtime_error("Ошибка: лишняя закрывающая скобка периода.");
            }

            if (result.periodDigits.empty()) {
                throw std::runtime_error("Ошибка: период не может быть пустым.");
            }

            periodClosed = true;
            ++pos;
            continue;
        }
        int digit = readOneDigit(text, pos, base);
        if (part == 0) {
            result.integerDigits.push_back(digit);
        } else if (part == 1) {
            result.fractionDigits.push_back(digit);
        } else {
            result.periodDigits.push_back(digit);
        }
    }

    if (result.integerDigits.empty()) {
        throw std::runtime_error("Ошибка: отсутствует целая часть числа.");
    }

    if (result.hasPoint && result.fractionDigits.empty() && !result.hasPeriod) {
        throw std::runtime_error("Ошибка: после точки должна быть дробная часть или период.");
    }

    if (result.hasPeriod && !periodClosed) {
        throw std::runtime_error("Ошибка: не закрыта скобка периода.");
    }

    if (result.hasPeriod && result.periodDigits.empty()) {
        throw std::runtime_error("Ошибка: период не может быть пустым.");
    }

    return result;
}

int BaseParser::readOneDigit(const std::string& text, int& pos, int base) {
    char ch = text[pos];

    int value = -1;
    if (ch >= '0' && ch <= '9') {
        value = ch - '0';
        ++pos;
    } else if (ch >= 'A' && ch <= 'Z') {
        value = ch - 'A' + 10;
        ++pos;
    } else if (ch >= 'a' && ch <= 'z') {
        value = ch - 'a' + 10;
        ++pos;
    } else if (ch == '[') {
        ++pos;
        if (pos >= static_cast<int>(text.size()) || text[pos] == ']') {
            throw std::runtime_error("Ошибка: внутри квадратных скобок должна быть цифра.");
        }
        std::string inside;
        while (pos < static_cast<int>(text.size()) && text[pos] != ']') {
            inside.push_back(text[pos]);
            ++pos;
        }
        if (pos == static_cast<int>(text.size())) {
            throw std::runtime_error("Ошибка: не закрыта квадратная скобка.");
        }
        ++pos;
        if (inside.size() == 1) {
            char symbol = inside[0];

            if (symbol >= '0' && symbol <= '9') {
                value = symbol - '0';
            } else if (symbol >= 'A' && symbol <= 'Z') {
                value = symbol - 'A' + 10;
            } else if (symbol >= 'a' && symbol <= 'z') {
                value = symbol - 'a' + 10;
            } else {
                throw std::runtime_error("Ошибка: внутри квадратных скобок недопустимый символ.");
            }
        } else {
            for (char symbol : inside) {
                if (symbol < '0' || symbol > '9') {
                    throw std::runtime_error("Ошибка: внутри квадратных скобок должно быть число или одна буква.");
                }
            }

            value = std::stoi(inside);
        }
    } else {
        throw std::runtime_error("Ошибка: недопустимый символ в позиции "
                                 + std::to_string(pos + 1) + ".");
    }

    if (value >= base) {
        throw std::runtime_error("Ошибка: цифра "
                                 + std::to_string(value)
                                 + " недопустима в системе счисления с основанием "
                                 + std::to_string(base) + ".");
    }
    return value;
}

BigInteger BaseParser::digitsToInteger(const std::vector<int>& digits, int base) {
    BigInteger result(0);

    for (int digit : digits) {
        result *= base;
        result += BigInteger(static_cast<unsigned long long>(digit));
    }

    return result;
}
