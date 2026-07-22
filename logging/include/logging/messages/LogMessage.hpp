#ifndef DESKTOP_MANAGER_LOGMESSAGE_HPP
#define DESKTOP_MANAGER_LOGMESSAGE_HPP
#include <string>

#include "Severity.hpp"
#include "SourcePath.hpp"
#include "Timestamp.hpp"

namespace Logging
{
    struct LogMessage
    {
        LogMessage(const SourcePath& path, const Severity severity, const std::string& message) :
            timestamp(std::chrono::system_clock::now()), source_path(path),
            severity(severity), message(message) {}

        Timestamp timestamp;
        SourcePath source_path;
        Severity severity;
        std::string message;
    };

    typedef std::shared_ptr<LogMessage> LogMessageHandle;
}

#endif //DESKTOP_MANAGER_LOGMESSAGE_HPP