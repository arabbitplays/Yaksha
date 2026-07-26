#ifndef DESKTOP_MANAGER_LOGCONFIGURATION_HPP
#define DESKTOP_MANAGER_LOGCONFIGURATION_HPP
#include <vector>

#include <logging/targets/Target.hpp>

namespace Logging
{
    struct LogConfiguration
    {
        friend class LogConfigurationBuilder;

        std::vector<TargetHandle> getTargets()
        {
            return targets;
        }
    private:
        std::vector<TargetHandle> targets{};
    };

    typedef std::shared_ptr<LogConfiguration> LogConfigurationHandle;
}

#endif //DESKTOP_MANAGER_LOGCONFIGURATION_HPP