#ifndef DESKTOP_MANAGER_LOGGER_HPP
#define DESKTOP_MANAGER_LOGGER_HPP
#include <logging/configuration/LogConfiguration.hpp>

namespace Logging
{
    class Logger
    {
    public:
        explicit Logger(const LogConfigurationHandle& config);
        virtual ~Logger() = default;

        void trace(const std::string& message);
        void debug(const std::string& message);
        void info(const std::string& message);
        void warn(const std::string& message);
        void error(const std::string& message);
        void critical(const std::string& message);

        virtual void log(const std::string& message, const Severity severity) = 0;
    protected:
        void publish(const LogMessageHandle& message) const;

    private:
        LogConfigurationHandle config;
    };

    typedef std::shared_ptr<Logger> LoggerHandle;
} // Logging

#endif //DESKTOP_MANAGER_LOGGER_HPP