#ifndef YAKSHA_MONITORLAYOUTFACTORY_HPP
#define YAKSHA_MONITORLAYOUTFACTORY_HPP
#include <string>

#include "model/MonitorLayout.hpp"

class MonitorLayoutFactory
{
public:
    static MonitorLayout getMonitorLayoutForCurrSystem()
    {
        std::string hostname = getHostname();
        MonitorLayout monitorLayout;
        if (hostname == "nix-desktop")
        {
            monitorLayout.tryAddingMonitorToRing("HDMI-A-3");
            monitorLayout.tryAddingMonitorToRing("DP-2");
            monitorLayout.tryAddingMonitorToRing("HDMI-A-4");
        }
        else if (hostname == "nix-laptop")
        {
            monitorLayout.tryAddingMonitorToRing("eDP-1");
            monitorLayout.tryAddingMonitorToRing("HDMI-A-1");
        }

        return monitorLayout;
    }

private:
    static std::string getHostname()
    {
        char hostname[255];
        if (gethostname(hostname, sizeof(hostname)) == 0)
        {
            return hostname;
        }
        return "";
    }
};

#endif //YAKSHA_MONITORLAYOUTFACTORY_HPP
