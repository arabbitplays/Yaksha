#ifndef DESKTOP_MANAGER_THEMESERVICE_HPP
#define DESKTOP_MANAGER_THEMESERVICE_HPP
#include <string>
#include <unordered_map>
#include <vector>

#include "bindings/HyprlandBinding.hpp"
#include "core/ShellActuator.hpp"
#include "model/Theme.hpp"

class ThemeService
{
public:
    ThemeService(const ShellActuatorHandle& shell_actuator, HyprlandBindingHandle hyprland_binding);
    ~ThemeService() = default;

    void setTheme(const std::string& theme_name);
private:
    void setWallpaperAll(const std::string& name) const;
    void setWallpaper(const std::string& name, const std::string& monitor_name) const;
    void setKittyTheme(const std::string& name) const;
    void setNvimTheme(const std::string& name);
    void setHyprTheme(const std::string& name) const;
    void setWaybarTheme(const std::string& name) const;

    static constexpr char* WALLPAPER_DIR = "~/Resources/Wallpapers";
    static constexpr char* KITTY_THEME_FILE = "~/.config/kitty/current-theme.conf";
    static constexpr char* KITTY_THEME_DIR = "~/.config/kitty/themes";
    static constexpr char* NVIM_THEME_FILE = "~/.config/nvim/current-theme";
    static constexpr char* HYPR_THEME_FILE = "~/.config/hypr/config/current-theme.conf";
    static constexpr char* HYPR_THEME_DIR = "~/.config/hypr/themes";
    static constexpr char* WAYBAR_THEME_FILE = "~/.config/waybar/current-theme.css";
    static constexpr char* WAYBAR_THEME_DIR = "~/.config/waybar/themes";

    static constexpr char* SWWW_OPTIONS = "--transition-type outer --transition-pos top-right --resize crop --transition-bezier=0.1,0.2,0.4,1.0 --transition-fps=60";

    ShellActuatorHandle shell_actuator;
    HyprlandBindingHandle hyprland_binding;

    std::vector<std::string> monitor_names;
    std::unordered_map<std::string, Theme> themes;
};


#endif //DESKTOP_MANAGER_THEMESERVICE_HPP