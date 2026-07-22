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
        return std::format_to(ctx.out(), "{}", severityToString(s));
    }
};

#endif //DESKTOP_MANAGER_SEVERITY_HPP
