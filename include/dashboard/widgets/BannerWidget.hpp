#ifndef YAKSHA_BANNERWIDGET_HPP
#define YAKSHA_BANNERWIDGET_HPP
#include "terminal_renderer/core/Vec3.hpp"
#include "terminal_renderer/model/color/RgbColor.hpp"
#include "terminal_renderer/widgets/Widget.hpp"

using namespace TerminalRenderer;

class BannerWidget : public Widget
{
public:
    BannerWidget() = default;
    ~BannerWidget() override = default;

    void onUpdate() override;

private:
    std::string banner = R"(
 .-.          .-         .'|                  .'|
  \ \        / /       .'  |                 <  |
   \ \      / /  __   <    |                  | |             __
    \ \    / /.:--.'.  |   | ____         _   | | .'''-.   .:--.'.
     \ \  / // |   \ | |   | \ .'       .' |  | |/.'''. \ / |   \ |
      \ `  / `" __ | | |   |/  .       .   | /|  /    | | `" __ | |
       \  /   .'.''| | |    /\  \    .'.'| |//| |     | |  .'.''| |
       / /   / /   | |_|   |  \  \ .'.'.-'  / | |     | | / /   | |_
   |`-' /    \ \._,\ '/'    \  \  \.'   \_.'  | '.    | '.\ \._,\ '/
    '..'      `--'  `"'------'  '---'         '---'   '---'`--'  `"
        )";

    ColorHandle getCurrColor();

public:

private:
    std::vector<Vec3> lerp_colors = {
        { 0.6,   0.15,  0.6   },
        { 0.942, 0.449, 0.942 },
        { 0.531, 0.559, 1.0   },
        { 0.168, 0.407, 0.871 },
        { 0.172, 0.022, 0.595 },
    };

    uint32_t curr_base_color = 0;
    const uint32_t lerp_frame_count = 10;
    uint32_t curr_lerp_frame = 0;
};


#endif //YAKSHA_BANNERWIDGET_HPP
