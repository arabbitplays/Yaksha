#ifndef DESKTOP_MANAGER_CLASSLOGGER_HPP
#define DESKTOP_MANAGER_CLASSLOGGER_HPP
#include "Logger.hpp"

namespace Logging
{
    class ClassLogger : public Logger
    {
    public:
        ClassLogger(const LogConfigurationHandle& config, const std::string& class_id);

    protected:
        void log(const std::string& raw_message, const Severity severity) override;

    private:
        SourcePath classpath;
    };
} // Logging

#endif //DESKTOP_MANAGER_CLASSLOGGER_HPP