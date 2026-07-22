#include <logging/targets/ConsoleTarget.hpp>

#include <iostream>

namespace Logging
{
    ConsoleTarget::ConsoleTarget() : Target() { }

    void ConsoleTarget::apply(const LogMessageHandle& message)
    {
        std::string output = std::format("{} {} {} - {}", message->timestamp.format(), message->severity, message->source_path.segments[0], message->message);
        std::cout << colorize(output, message->severity) << std::endl;
    }

    std::string ConsoleTarget::colorize(const std::string& data, Severity severity) {
        return ColorUtil::getColorCode(getColorForSeverity(severity)) + data + ColorUtil::getColorResetCode();
    }

    Color ConsoleTarget::getColorForSeverity(const Severity severity) {
        switch (severity) {
            case TRACE:
                return Color::DARK_GRAY;
            case DEBUG:
                return Color::GRAY;
            case INFO:
                return Color::WHITE;
            case WARN:
                return Color::YELLOW;
            case ERROR:
                return Color::RED;
            case CRITICAL:
                return Color::BRIGHT_RED;
        }
        return Color::WHITE;
    }
} // Logging
