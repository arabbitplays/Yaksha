#ifndef DESKTOP_MANAGER_THEME_HPP
#define DESKTOP_MANAGER_THEME_HPP
#include <string>

struct Theme
{
    std::string wallpaper_name;
    std::string kitty_theme;
    std::string nvim_theme;
    std::string hypr_theme;
    std::string waybar_theme;
};

#endif //DESKTOP_MANAGER_THEME_HPP
