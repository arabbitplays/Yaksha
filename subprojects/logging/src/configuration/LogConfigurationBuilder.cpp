#include <logging/configuration/LogConfigurationBuilder.hpp>

namespace Logging
{
    LogConfigurationBuilder::LogConfigurationBuilder()
    {
        config = std::make_shared<LogConfiguration>();
    }

    LogConfigurationBuilder& LogConfigurationBuilder::addTarget(const TargetHandle& target)
    {
        config->targets.push_back(target);
        return *this;
    }

    LogConfigurationBuilder& LogConfigurationBuilder::addRule(const FilterRuleHandle& rule, const TargetHandle& target)
    {
        target->addRule(rule);
        return *this;
    }

    LogConfigurationBuilder& LogConfigurationBuilder::addRule(std::string path_pattern, Severity min_severity,
        const TargetHandle& target)
    {
        const auto rule = std::make_shared<FilterRule>(path_pattern, min_severity);
        return addRule(rule, target);
    }

    LogConfigurationHandle LogConfigurationBuilder::build()
    {
        return config;
    }
} // Logging