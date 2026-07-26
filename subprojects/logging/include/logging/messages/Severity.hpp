#ifndef DESKTOP_MANAGER_SEVERITY_HPP
#define DESKTOP_MANAGER_SEVERITY_HPP
#include <format>
#include <string_view>

namespace Logging
{
    enum Severity
    {
        TRACE,
        DEBUG,
        INFO,
        WARN,
        ERROR,
        CRITICAL,
    };

    constexpr std::string_view severityNames[] = {
        "TRACE",
        "DEBUG",
        "INFO",
        "WARN",
        "ERROR",
        "CRITICAL"
    };

    inline const uint32_t MAX_SEVERITY_STRING_LENGTH = 8;

    inline std::string_view severityToString(Severity severity)
    {
        return severityNames[severity];
    }
}

template <>
struct std::formatter<Logging::Severity>
{
    constexpr auto parse(format_parse_context& ctx)
    {
        return ctx.begin();
    }

    auto format(Logging::Severity s, format_context& ctx) const
    {
        std::string_view severity_string = severityToString(s);
        std::string padding = "";
        for (uint32_t i = 0; i < Logging::MAX_SEVERITY_STRING_LENGTH - severity_string.size(); ++i)
        {
            padding += " ";
        }
        return std::format_to(ctx.out(), "[{}]{}", severity_string, padding);
    }
};

#endif //DESKTOP_MANAGER_SEVERITY_HPP
