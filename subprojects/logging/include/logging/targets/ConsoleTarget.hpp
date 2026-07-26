#ifndef DESKTOP_MANAGER_CONSOLETARGET_HPP
#define DESKTOP_MANAGER_CONSOLETARGET_HPP
#include "Target.hpp"
#include <logging/util/ColorUtil.hpp>

namespace Logging
{
    class ConsoleTarget final : public Target
    {
    public:
        ConsoleTarget();
        ~ConsoleTarget() override = default;

    protected:
        void apply(const LogMessageHandle& message) override;
        std::string colorize(const std::string& data, Severity severity);
        Color getColorForSeverity(const Severity severity);
    };
} // Logging

#endif //DESKTOP_MANAGER_CONSOLETARGET_HPP
