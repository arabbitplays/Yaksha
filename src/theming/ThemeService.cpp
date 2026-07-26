#include "../../include/theming/ThemeService.hpp"

#include "theming/ThemeController.hpp"
#include "util/FileUtil.hpp"
#include "util/MonitorUtil.hpp"

ThemeService::ThemeService(const ShellActuatorHandle& shell_actuator) : shell_actuator(shell_actuator)
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
    shell_actuator->executeShellCommand("swww img " + std::string(WALLPAPER_DIR) + "/" + name + " " + std::string(SWWW_OPTIONS));
}

void ThemeService::setWallpaper(const std::string& name, const std::string& monitor_name) const {
    shell_actuator->executeShellCommand("swww img " + std::string(WALLPAPER_DIR) + "/" + name + " " + std::string (SWWW_OPTIONS) + " -o " + monitor_name);
}

void ThemeService::setKittyTheme(const std::string& name) const
{
    std::string src = std::string(KITTY_THEME_DIR) + "/" + name;
    std::string dst = std::string(KITTY_THEME_FILE);
    FileUtil::copyFile(src, dst);
    shell_actuator->executeShellCommand("kill -USR1 $(pidof kitty)");
}

void ThemeService::setNvimTheme(const std::string& name) {
    FileUtil::overwriteFile(std::string(NVIM_THEME_FILE), name);
}

void ThemeService::setHyprTheme(const std::string& name) const
{
    std::string src = std::string(HYPR_THEME_DIR) + "/" + name;
    std::string dst = std::string(HYPR_THEME_FILE);
    FileUtil::copyFile(src, dst);
    shell_actuator->executeShellCommand("hyprctl reload");
}

void ThemeService::setWaybarTheme(const std::string& name) const
{
    std::string src = std::string(WAYBAR_THEME_DIR) + "/" + name;
    std::string dst = std::string(WAYBAR_THEME_FILE);
    FileUtil::copyFile(src, dst);

    shell_actuator->executeShellCommand("pkill waybar");
    std::system("waybar > /dev/null 2>&1 &"); // TODO is this needed outside of the shell actuator?
}
