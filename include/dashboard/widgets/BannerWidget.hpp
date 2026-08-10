#ifndef YAKSHA_BANNERWIDGET_HPP
#define YAKSHA_BANNERWIDGET_HPP
#include "terminal_renderer/widgets/Widget.hpp"

using namespace TerminalRenderer;

class BannerWidget : public Widget
{
public:
    BannerWidget();
    ~BannerWidget() = default;

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
};


#endif //YAKSHA_BANNERWIDGET_HPP
