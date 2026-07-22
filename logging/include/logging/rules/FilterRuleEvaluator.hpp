#ifndef DESKTOP_MANAGER_RULEEVALUATOR_HPP
#define DESKTOP_MANAGER_RULEEVALUATOR_HPP
#include "FilterRule.hpp"
#include <logging/messages/LogMessage.hpp>

namespace Logging
{
    class FilterRuleEvaluator
    {
    public:
        FilterRuleEvaluator() = default;
        ~FilterRuleEvaluator() = default;

        bool evaluate(const LogMessageHandle& log_message, const FilterRuleHandle& rule);

    private:
        static bool containsPath(const SourcePath& path_pattern, const SourcePath& path);
    };
} // Logging

#endif //DESKTOP_MANAGER_RULEEVALUATOR_HPP