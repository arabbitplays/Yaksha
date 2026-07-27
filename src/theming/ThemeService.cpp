#include "../../include/theming/ThemeService.hpp"

#include <utility>

#include "theming/ThemeController.hpp"
#include "util/FileUtil.hpp"
#include "util/MonitorUtil.hpp"

ThemeService::ThemeService(const ShellActuatorHandle& shell_actuator,
                           HyprlandBindingHandle hyprland_binding,
                           SwwwBindingHandle swww_binding,
                           KittyBindingHandle kitty_binding,
                           WaybarBindingHandle waybar_binding)
    : shell_actuator(shell_actuator),
      hyprland_binding(std::move(hyprland_binding)),
      swww_binding(std::move(swww_binding)),
      kitty_binding(std::move(kitty_binding)),
      waybar_binding(std::move(waybar_binding))
{
    monitor_names = MonitorUtil::getMonitorNamesForCurrSystem();

    themes["tokyo"] = {
        .wallpaper_name = "tokyo_night.jpg",
        .kitty_theme = "tokyo_night.conf",
        .nvim_theme = "tokyo",
        .hypr_theme = "tokyo.conf",
        .waybar_theme = "tokyo.css",
    };

    themes["forest"] = {
        .wallpaper_name = "nier.jpg",
        .kitty_theme = "gruvbox_dark.conf",
        .nvim_theme = "forest",
        .hypr_theme = "forest.conf",
        .waybar_theme = "forest.css",
    };
}

void ThemeService::setTheme(const std::string& theme_name)
{
    if (themes.contains(theme_name)) {
        Theme& theme = themes.at(theme_name);
        setKittyTheme(theme.kitty_theme);
        setNvimTheme(theme.nvim_theme);
        setHyprTheme(theme.hypr_theme);
        setWaybarTheme(theme.waybar_theme);
        setWallpaperAll(theme.wallpaper_name);
    } else {
        throw std::runtime_error("Theme " + theme_name + " not found");
    }
}

void ThemeService::setWallpaperAll(const std::string& name) const
{
    swww_binding->setWallpaper(std::string(WALLPAPER_DIR) + "/" + name);
}

void ThemeService::setWallpaper(const std::string& name, const std::string& monitor_name) const {
    swww_binding->setWallpaper(std::string(WALLPAPER_DIR) + "/" + name, monitor_name);
}

void ThemeService::setKittyTheme(const std::string& name) const
{
    std::string src = std::string(KITTY_THEME_DIR) + "/" + name;
    std::string dst = std::string(KITTY_THEME_FILE);
    FileUtil::copyFile(shell_actuator, src, dst);
    kitty_binding->reload();
}

void ThemeService::setNvimTheme(const std::string& name) {
    FileUtil::overwriteFile(std::string(NVIM_THEME_FILE), name);
}

void ThemeService::setHyprTheme(const std::string& name) const
{
    std::string src = std::string(HYPR_THEME_DIR) + "/" + name;
    std::string dst = std::string(HYPR_THEME_FILE);
    FileUtil::copyFile(shell_actuator, src, dst);
    hyprland_binding->reload();
}

void ThemeService::setWaybarTheme(const std::string& name) const
{
    std::string src = std::string(WAYBAR_THEME_DIR) + "/" + name;
    std::string dst = std::string(WAYBAR_THEME_FILE);
    FileUtil::copyFile(shell_actuator, src, dst);
    waybar_binding->restart();
}
