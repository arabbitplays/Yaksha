#include <logging/targets/ConsoleTarget.hpp>

#include <iostream>

namespace Logging
{
    ConsoleTarget::ConsoleTarget() : Target() { }

    void ConsoleTarget::apply(const LogMessageHandle& message)
    {
        std::cout << message << std::endl;
    }
} // Logging