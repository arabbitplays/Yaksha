#ifndef DESKTOP_MANAGER_LOGMANAGER_HPP
#define DESKTOP_MANAGER_LOGMANAGER_HPP
#include <logging/configuration/LogConfiguration.hpp>
#include <logging/logger/ClassLogger.hpp>

namespace Logging
{
    class LogManager
    {
    public:
        static void setLogConfiguration(const LogConfigurationHandle& config)
        {
            if (config != nullptr)
            {
                throw std::runtime_error("Logging config can only be set once");
            }
            LogManager::config = config;
        }

        template <typename T>
        static LoggerHandle getClassLogger()
        {
            return std::make_shared<ClassLogger>(config, typeid(T).name());
        }

    private:
        static LogConfigurationHandle config;
    };
}

#endif //DESKTOP_MANAGER_LOGMANAGER_HPP