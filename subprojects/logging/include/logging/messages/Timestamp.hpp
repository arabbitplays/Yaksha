#ifndef DESKTOP_MANAGER_TIMESTAMP_HPP
#define DESKTOP_MANAGER_TIMESTAMP_HPP
#include <chrono>

namespace Logging
{
    using DateTime = std::chrono::system_clock::time_point;
    struct Timestamp
    {
        Timestamp(const DateTime& date_time) : date_time(date_time) {}
        DateTime date_time;

        std::string format()
        {
            auto secs = std::chrono::floor<std::chrono::seconds>(date_time);
            return std::format("{:%Y-%m-%d %H:%M:%S}", secs);
        }
    };
}

#endif //DESKTOP_MANAGER_TIMESTAMP_HPP