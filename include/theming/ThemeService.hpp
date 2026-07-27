#ifndef DESKTOP_MANAGER_THEMESERVICE_HPP
#define DESKTOP_MANAGER_THEMESERVICE_HPP
#include <string>
#include <unordered_map>
#include <vector>

#include "bindings/HyprlandBinding.hpp"
#include "bindings/KittyBinding.hpp"
#include "bindings/SwwwBinding.hpp"
#include "bindings/WaybarBinding.hpp"
#include "core/ShellActuator.hpp"
#include "model/Theme.hpp"

class ThemeService
{
public:
    ThemeService(const ShellActuatorHandle& shell_actuator,
                 HyprlandBindingHandle hyprland_binding,
                 SwwwBindingHandle swww_binding,
                 KittyBindingHandle kitty_binding,
                 WaybarBindingHandle waybar_binding);
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

    ShellActuatorHandle shell_actuator;
    HyprlandBindingHandle hyprland_binding;
    SwwwBindingHandle swww_binding;
    KittyBindingHandle kitty_binding;
    WaybarBindingHandle waybar_binding;

    std::vector<std::string> monitor_names;
    std::unordered_map<std::string, Theme> themes;
};


#endif //DESKTOP_MANAGER_THEMESERVICE_HPP
