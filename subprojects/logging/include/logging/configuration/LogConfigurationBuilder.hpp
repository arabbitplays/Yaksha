#ifndef DESKTOP_MANAGER_LOGCONFIGURATIONBUILDER_HPP
#define DESKTOP_MANAGER_LOGCONFIGURATIONBUILDER_HPP
#include "LogConfiguration.hpp"
#include <logging/targets/Target.hpp>

namespace Logging
{
    class LogConfigurationBuilder
    {
    public:
        LogConfigurationBuilder();

        LogConfigurationBuilder& addTarget(const TargetHandle& target);
        LogConfigurationBuilder& addRule(const FilterRuleHandle& rule, const TargetHandle& target);
        LogConfigurationBuilder& addRule(std::string path_pattern, Severity min_severity, const TargetHandle& target);

        LogConfigurationHandle build();

    private:
        LogConfigurationHandle config{};
    };
} // Logging

#endif //DESKTOP_MANAGER_LOGCONFIGURATIONBUILDER_HPP