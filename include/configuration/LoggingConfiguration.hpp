#ifndef DESKTOP_MANAGER_LOGGINGCONFIGURATION_HPP
#define DESKTOP_MANAGER_LOGGINGCONFIGURATION_HPP
#include "logging/configuration/LogConfigurationBuilder.hpp"
#include <logging/LogManager.hpp>
#include "logging/targets/ConsoleTarget.hpp"

class LoggingConfiguration
{
public:
    static void configure()
    {
        auto console_target = std::make_shared<Logging::ConsoleTarget>();
        auto log_config = Logging::LogConfigurationBuilder()
            .addRule("*", Logging::TRACE, console_target)
            .addTarget(console_target).build();
        Logging::LogManager::setLogConfiguration(log_config);
    }
};

#endif //DESKTOP_MANAGER_LOGGINGCONFIGURATION_HPP