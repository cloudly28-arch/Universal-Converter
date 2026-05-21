#include "BaseConverter.h"

#include "BaseParser.h"
#include "baseformatter.h"

#include <cctype>
#include <stdexcept>

ConvertResult BaseConverter::convert(const std::string& input, const std::string& pText, const std::string& qText) {
    ConvertResult result;
    try {
        int p = parseBase(pText, "исходной");
        int q = parseBase(qText, "целевой");
        BigFraction value = BaseParser::parse(input, p);
        FormatResult formatted = BaseFormatter::format(value, q);

        result.ok = true;
        result.output = formatted.text;
        if (formatted.truncated) {
            result.message = "Полная запись результата слишком длинная. Показаны первые " +
                             std::to_string(BaseFormatter::MAX_OUTPUT_LENGTH) + " символов.";
        } else {
            result.message = "Готово.";
        }
    } catch (const std::exception& error) {
        result.ok = false;
        result.output.clear();
        result.message = error.what();
    }
    return result;
}

int BaseConverter::parseBase(const std::string& text, const std::string& name) {
    if (text.empty()) {
        throw std::runtime_error("Ошибка: основание " + name + " системы должно быть целым числом от 2 до 500.");
    }
    for (char ch : text) {
        if (!std::isdigit(static_cast<unsigned char>(ch))) {
            throw std::runtime_error("Ошибка: основание " + name + " системы должно быть целым числом от 2 до 500.");
        }
    }
    int value = std::stoi(text);
    if (value < 2 || value > 500) {
        throw std::runtime_error("Ошибка: основание " + name + " системы должно быть целым числом от 2 до 500.");
    }
    return value;
}
