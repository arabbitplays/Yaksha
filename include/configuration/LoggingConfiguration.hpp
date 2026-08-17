#ifndef DESKTOP_MANAGER_LOGGINGCONFIGURATION_HPP
#define DESKTOP_MANAGER_LOGGINGCONFIGURATION_HPP
#include "logging/configuration/LogConfigurationBuilder.hpp"
#include <logging/LogManager.hpp>
#include <logging/targets/ConsoleTarget.hpp>
#include <logging/targets/InMemoryTarget.hpp>

class LoggingConfiguration
{
public:
    static void configure(bool verbose)
    {
        Logging::Severity min_severity = verbose ? Logging::TRACE : Logging::INFO;
        auto console_target = std::make_shared<Logging::ConsoleTarget>();
        in_memory_target = std::make_shared<Logging::InMemoryTarget>(30);
        auto log_config = Logging::LogConfigurationBuilder()
            .addRule("*", min_severity, console_target)
            .addTarget(console_target)
            .addRule("*", WARN, in_memory_target)
            .addTarget(in_memory_target)
            .build();
        Logging::LogManager::setLogConfiguration(log_config);
    }

    inline static std::shared_ptr<Logging::InMemoryTarget> in_memory_target = nullptr;
};

#endif //DESKTOP_MANAGER_LOGGINGCONFIGURATION_HPP