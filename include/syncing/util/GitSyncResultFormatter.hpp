#ifndef DESKTOP_MANAGER_GITSYNCRESULTFORMATTER_HPP
#define DESKTOP_MANAGER_GITSYNCRESULTFORMATTER_HPP

#include <sstream>
#include <string>

#include "../model/GitSyncResult.hpp"

class GitSyncResultFormatter
{
public:
    static std::string format(const GitSyncResult& result)
    {
        std::ostringstream out;
        out << "[" << result.name << "] "
            << formatPull(result.pullResult) << " | "
            << formatPush(result.pushResult);
        return out.str();
    }

private:
    static constexpr const char* GREEN  = "\033[0;32m";
    static constexpr const char* RED    = "\033[0;31m";
    static constexpr const char* YELLOW = "\033[0;33m";
    static constexpr const char* RESET  = "\033[0m";

    static std::string colorize(const std::string& text, const char* color)
    {
        return std::string(color) + text + RESET;
    }

    static std::string formatPull(PullResult result)
    {
        switch (result)
        {
            case PullResult::SUCCESS: return colorize("Pull successful", GREEN);
            case PullResult::FAILED:  return colorize("Pull failed", RED);
        }
        return {};
    }

    static std::string formatPush(PushResult result)
    {
        switch (result)
        {
            case PushResult::SUCCESS:           return colorize("Push successful", GREEN);
            case PushResult::NOTHING_TO_COMMIT: return colorize("Nothing to commit", YELLOW);
            case PushResult::FAILED:            return colorize("Push failed", RED);
        }
        return {};
    }
};

#endif // DESKTOP_MANAGER_GITSYNCRESULTFORMATTER_HPP
