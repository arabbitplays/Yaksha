#ifndef YAKSHA_STRINGUTIL_H
#define YAKSHA_STRINGUTIL_H
#include <algorithm>
#include <string>
#include <vector>

class StringUtil
{
public:
    static std::string removeWhitespace(std::string input)
    {
        std::erase_if(input, [](char ch)
        {
            return ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r';
        });
        return input;
    }

    static std::vector<std::string> split(const std::string& input, const std::string& delimiter,
                                          bool remove_whitespace = false)
    {
        std::vector<std::string> result{};
        if (delimiter.empty())
        {
            std::string token = remove_whitespace ? removeWhitespace(input) : input;
            if (!token.empty())
                result.push_back(std::move(token));
            return result;
        }

        size_t start = 0;
        while (start <= input.size())
        {
            size_t pos = input.find(delimiter, start);
            size_t end = pos == std::string::npos ? input.size() : pos;

            std::string token = input.substr(start, end - start);
            if (remove_whitespace)
                token = removeWhitespace(std::move(token));
            if (!token.empty())
                result.push_back(std::move(token));

            if (pos == std::string::npos)
                break;
            start = pos + delimiter.size();
        }
        return result;
    }

    static std::vector<std::string> split(const std::string& input, const char delimiter,
                                          bool remove_whitespace = false)
    {
        return split(input, std::string(1, delimiter), remove_whitespace);
    }
};

#endif //YAKSHA_STRINGUTIL_H
