#ifndef DESKTOP_MANAGER_COLORUTIL_HPP
#define DESKTOP_MANAGER_COLORUTIL_HPP
#include <regex>
#include <string>

namespace Logging
{
    enum Color
    {
        RED,
        YELLOW,
        GREEN,
        GRAY,
        DARK_GRAY,
        BRIGHT_RED,
        WHITE,
    };

    class ColorUtil
    {
    public:
        static std::string getColorCode(Color color)
        {
            switch (color)
            {
            case RED:
                return "\033[31m";
            case YELLOW:
                return "\033[33m";
            case GREEN:
                return "\033[32m";
            case GRAY:
                return "\033[37m";
            case DARK_GRAY:
                return "\033[90m";
            case BRIGHT_RED:
                return "\033[91m";
            case WHITE:
                return "\033[97m";
            default:
                return "\033[39m";
            }
        }

        static std::string getColorResetCode()
        {
            return "\033[0m";
        }

        static std::string stripAnsiCodes(const std::string& data)
        {
            static const std::regex ansi_pattern("\x1b\\[[0-9;]*[a-zA-Z]");
            return std::regex_replace(data, ansi_pattern, "");
        }
    };
}

#endif //DESKTOP_MANAGER_COLORUTIL_HPP
