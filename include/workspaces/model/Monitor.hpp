#ifndef YAKSHA_MONITOR_H
#define YAKSHA_MONITOR_H
#include <cstdint>
#include <memory>
#include <string>

struct Monitor
{
    Monitor(const uint32_t physical_id, const std::string& name) : physical_id(physical_id), name(name) {}
    uint32_t physical_id;
    std::string name;
};

typedef std::shared_ptr<Monitor> MonitorHandle;

#endif //YAKSHA_MONITOR_H