#include <logging/targets/Target.hpp>

namespace Logging
{
    void Target::log(const LogMessageHandle& message)
    {
        if (appliesTo(message))
        {
            apply(message);
        }
    }

    void Target::addRule(const FilterRuleHandle& rule)
    {
        rules.push_back(rule);
    }

    bool Target::appliesTo(const LogMessageHandle& message)
    {
        if (rules.empty())
        {
            return true;
        }

        for (const auto& rule : rules)
        {
            if (rule_evaluator.evaluate(message, rule))
            {
                return true;
            }
        }

        return false;
    }
}
