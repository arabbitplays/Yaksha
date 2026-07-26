#include <logging/logger/Logger.hpp>

namespace Logging
{
    Logger::Logger(const LogConfigurationHandle& config) : config(config) { }

    void Logger::trace(const std::string& message)
    {
        log(message, Severity::TRACE);
    }

    void Logger::debug(const std::string& message)
    {
        log(message, Severity::DEBUG);
    }

    void Logger::info(const std::string& message)
    {
        log(message, Severity::INFO);
    }

    void Logger::warn(const std::string& message)
    {
        log(message, Severity::WARN);
    }

    void Logger::error(const std::string& message)
    {
        log(message, Severity::ERROR);
    }

    void Logger::critical(const std::string& message)
    {
        log(message, Severity::CRITICAL);
    }

    void Logger::publish(const LogMessageHandle& message) const
    {
        for (const auto& target : config->getTargets())
        {
            target->log(message);
        }
    }
} // Logging