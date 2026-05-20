#pragma once

#include <string>

struct ConvertResult {
    bool ok = false;
    std::string output;
    std::string message;
};

class BaseConverter {
public:
    static ConvertResult convert(const std::string& input, const std::string& pText, const std::string& qText);

private:
    static int parseBase(const std::string& text, const std::string& name);
};
