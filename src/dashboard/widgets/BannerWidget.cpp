#include "../../../include/dashboard/widgets/BannerWidget.hpp"

#include "terminal_renderer/builder/SceneBuilder.hpp"

BannerWidget::BannerWidget()
{
    root = SceneBuilder::text(banner, std::nullopt, std::nullopt, {.flow_mode = TextFlowMode::CUTOFF}).build();
}