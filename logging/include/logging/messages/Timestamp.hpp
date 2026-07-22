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
    };
}

#endif //DESKTOP_MANAGER_TIMESTAMP_HPP