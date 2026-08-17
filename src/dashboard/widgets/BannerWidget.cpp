#include "../../../include/dashboard/widgets/BannerWidget.hpp"

#include "terminal_renderer/builder/SceneBuilder.hpp"
#include "terminal_renderer/model/color/RgbColor.hpp"

void BannerWidget::onUpdate()
{
    root = SceneBuilder::text(banner, getCurrColor(), std::nullopt, {.flow_mode = TextFlowMode::CUTOFF}).build();
}

ColorHandle BannerWidget::getCurrColor()
{
    Vec3 base_color = lerp_colors.at(curr_base_color);
    Vec3 target_color = lerp_colors.at((curr_base_color + 1) % lerp_colors.size());
    Vec3 diff = target_color - base_color;
    Vec3 color = base_color + static_cast<float>(curr_lerp_frame) / static_cast<float>(lerp_frame_count) * diff;
    curr_lerp_frame++;
    if (curr_lerp_frame == lerp_frame_count)
    {
        curr_lerp_frame = 0;
        curr_base_color = (curr_base_color + 1) % lerp_colors.size();
    }
    return RgbColor::create(color);
}
