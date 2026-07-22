#include "include/controller/ThemeController.hpp"
#include "io/CommandParser.hpp"
#include "util/MonitorUtil.hpp"
#include "util/ShellUtil.hpp"
#include "util/FileUtil.hpp"
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>

namespace fs = std::filesystem;

ThemeController::ThemeController() {
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

std::string ThemeController::getKeyword() const {
    return "theme";
}

std::string ThemeController::execute(io::CommandHandle& cmd) {
    if (cmd->args.size() != 1) {
        throw std::runtime_error("Command 'theme' requires one string argument\n");
    };

    std::string theme_name = cmd->args[0];

    if (themes.contains(theme_name)) {
        Theme& theme = themes.at(theme_name);
        setKittyTheme(theme.kitty_theme);
        setNvimTheme(theme.nvim_theme);
        setHyprTheme(theme.hypr_theme);
        setWaybarTheme(theme.waybar_theme);
        setWallpaperAll(theme.wallpaper_name);
    } else {
        throw std::runtime_error("Command " + getKeyword() + " " + cmd->args[0] + " does not exist!");
    }

    return "ok";
}

void ThemeController::setWallpaperAll(const std::string& name) {
    std::string output = ShellUtil::executeShellCommand("swww img " + std::string(WALLPAPER_DIR) + "/" + name + " " + std::string(SWWW_OPTIONS));
    ShellUtil::printShellOutput(output);
}

void ThemeController::setWallpaper(const std::string name, const std::string& monitor_name) const {
    std::string output = ShellUtil::executeShellCommand("swww img " + std::string(WALLPAPER_DIR) + "/" + name + " " + std::string (SWWW_OPTIONS) + " -o " + monitor_name);
    ShellUtil::printShellOutput(output);
} 

void ThemeController::setKittyTheme(const std::string& name) {
    std::string src = std::string(KITTY_THEME_DIR) + "/" + name;
    std::string dst = std::string(KITTY_THEME_FILE);
    FileUtil::copyFile(src, dst);    
    std::string output = ShellUtil::executeShellCommand("kill -USR1 $(pidof kitty)");
    ShellUtil::printShellOutput(output);
}

void ThemeController::setNvimTheme(const std::string& name) {
    FileUtil::overwriteFile(std::string(NVIM_THEME_FILE), name);
}

void ThemeController::setHyprTheme(const std::string& name) {
    std::string src = std::string(HYPR_THEME_DIR) + "/" + name;
    std::string dst = std::string(HYPR_THEME_FILE);
    FileUtil::copyFile(src, dst);    
    std::string output = ShellUtil::executeShellCommand("hyprctl reload");
    ShellUtil::printShellOutput(output);
}

void ThemeController::setWaybarTheme(const std::string& name) {
    std::string src = std::string(WAYBAR_THEME_DIR) + "/" + name;
    std::string dst = std::string(WAYBAR_THEME_FILE);
    FileUtil::copyFile(src, dst);    

    std::string output = ShellUtil::executeShellCommand("pkill waybar");
    std::system("waybar > /dev/null 2>&1 &");
    ShellUtil::printShellOutput(output);
}
