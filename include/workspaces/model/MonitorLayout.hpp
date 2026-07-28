#ifndef YAKSHA_MONITORLAYOUT_HPP
#define YAKSHA_MONITORLAYOUT_HPP
#include <string>
#include <unordered_map>

#include "WindowMovement.hpp"


struct MonitorLayout
{
private:
    typedef std::unordered_map<WindowMovement, std::string> NeighborMap;

    Logging::LoggerHandle logger = Logging::LogManager::getClassLogger<MonitorLayout>();
    std::unordered_map<std::string, NeighborMap> layout;
    std::string right_monitor;

public:
    void tryAddingMonitorToRing(const std::string& monitor_name)
    {
        if (layout.contains(monitor_name))
            return;

        if (right_monitor.empty())
        {
            assert(layout.empty());
            layout[monitor_name] = NeighborMap();
            layout[monitor_name][LEFT] = monitor_name;
            layout[monitor_name][RIGHT] = monitor_name;
        } else
        {
            layout[monitor_name] = NeighborMap();
            layout[monitor_name][LEFT] = right_monitor;
            std::string left_monitor = layout[right_monitor][RIGHT];
            layout[monitor_name][RIGHT] = left_monitor;
            layout[left_monitor][LEFT] = monitor_name;
            layout[right_monitor][RIGHT] = monitor_name;
        }
        right_monitor = monitor_name;
        logger->debug("Added monitor " + monitor_name + " to the right of the layout");
    }

    std::optional<std::string> getAdjacentMonitorName(const std::string& monitor_name, WindowMovement movement) const
    {
        if (!layout.contains(monitor_name))
        {
            throw std::invalid_argument("Unknown monitor name " + monitor_name + " for this layout");
        }

        if (!layout.at(monitor_name).contains(movement))
        {
            return std::nullopt;
        }

        return layout.at(monitor_name).at(movement);
    }
};

#endif //YAKSHA_MONITORLAYOUT_HPP