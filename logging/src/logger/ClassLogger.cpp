#include <logging/logger/ClassLogger.hpp>

namespace Logging
{
    ClassLogger::ClassLogger(const LogConfigurationHandle& config, const std::string& class_id)
        : Logger(config), classpath(SourcePath(class_id))
    {
    }

    void ClassLogger::log(const std::string& raw_message, const Severity severity)
    {
        const auto message = std::make_shared<LogMessage>(classpath, severity, raw_message);
        publish(message);
    }
} // Logging
