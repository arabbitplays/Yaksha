#include <logging/targets/ConsoleTarget.hpp>

#include <iostream>

namespace Logging
{
    ConsoleTarget::ConsoleTarget() : Target() { }

    void ConsoleTarget::apply(const LogMessageHandle& message)
    {
        std::cout << std::format("[{}] {} - {}", message->severity, message->source_path.segments[0], message->message) << std::endl;
    }
} // Logging