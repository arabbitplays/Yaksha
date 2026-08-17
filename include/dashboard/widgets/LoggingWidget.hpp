#ifndef YAKSHA_LOGGINGWIDGET_HPP
#define YAKSHA_LOGGINGWIDGET_HPP
#include "logging/messages/LogMessage.hpp"
#include "terminal_renderer/nodes/text_node/TextNode.hpp"
#include "terminal_renderer/widgets/Widget.hpp"

using namespace TerminalRenderer;
using namespace Logging;

class LoggingWidget : public Widget
{
public:
    LoggingWidget();
    ~LoggingWidget() override = default;

    void onUpdate() override;
    void addLogMessageSegment(const LogMessageHandle& message) const;

private:
    std::shared_ptr<TextNode> text_node;
};


#endif //YAKSHA_LOGGINGWIDGET_HPP