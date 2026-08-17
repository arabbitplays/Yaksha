#include "../../../include/dashboard/widgets/LoggingWidget.hpp"

#include "configuration/LoggingConfiguration.hpp"
#include "logging/messages/LogMessage.hpp"
#include "terminal_renderer/builder/SceneBuilder.hpp"


LoggingWidget::LoggingWidget()
{
    text_node = SceneBuilder::text({.flow_mode = TextFlowMode::LINE_BREAK}).build();
    root = SceneBuilder::roundedBorder(0, 1).setChild(text_node).build();
}

void LoggingWidget::onUpdate()
{
    auto logging_target = LoggingConfiguration::in_memory_target;
    if (logging_target == nullptr)
        return;
    std::vector<LogMessageHandle> messages = logging_target->getLogMessages();

    text_node->clearTextSegments();
    text_node->appendTextSegment("Warnings and Errors:\n");
    for (const auto& message : messages)
    {
        addLogMessageSegment(message);
    }
}

void LoggingWidget::addLogMessageSegment(const LogMessageHandle& message) const
{
    ColorHandle color = message->severity == WARN
                            ? StandardColor::create(StandardColorType::YELLOW)
                            : StandardColor::create(StandardColorType::RED);
    text_node->appendTextSegment(std::format("{} {} - {}\n", message->timestamp.format(),
                                             message->source_path.segments.at(0), message->message), color);
}
