#ifndef YAKSHA_STRINGUTIL_H
#define YAKSHA_STRINGUTIL_H
#include <string>
#include <vector>

class StringUtil
{
public:
    static std::vector<std::string> split(const std::string& input, const char delimiter)
    {
        std::vector<std::string> result{};
        std::string curr_sub_str;
        for (char c : input)
        {
            if (c == delimiter)
            {
                if (curr_sub_str.empty())
                    continue;
                result.push_back(curr_sub_str);
                curr_sub_str.clear();
            }
            else
            {
                curr_sub_str += c;
            }
        }
        if (!curr_sub_str.empty())
        {
            result.push_back(curr_sub_str);
        }
        return result;
    }

    static std::vector<std::string> split(const std::string& input, const std::string& delimiter)
    {
        std::vector<std::string> result{};
        if (delimiter.empty())
        {
            if (!input.empty())
                result.push_back(input);
            return result;
        }

        size_t start = 0;
        while (start <= input.size())
        {
            size_t pos = input.find(delimiter, start);
            size_t end = pos == std::string::npos ? input.size() : pos;
            if (end > start)
                result.push_back(input.substr(start, end - start));
            if (pos == std::string::npos)
                break;
            start = pos + delimiter.size();
        }
        return result;
    }
};

#endif //YAKSHA_STRINGUTIL_H
