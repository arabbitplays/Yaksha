#ifndef DESKTOP_MANAGER_CONSOLETARGET_HPP
#define DESKTOP_MANAGER_CONSOLETARGET_HPP
#include "Target.hpp"

namespace Logging
{
    class ConsoleTarget final : public Target
    {
    public:
        ConsoleTarget();
        ~ConsoleTarget() override = default;

    protected:
        void apply(const LogMessageHandle& message) override;
    };
} // Logging

#endif //DESKTOP_MANAGER_CONSOLETARGET_HPP