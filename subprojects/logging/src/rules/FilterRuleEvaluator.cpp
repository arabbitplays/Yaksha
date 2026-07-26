
#include <logging/rules/FilterRuleEvaluator.hpp>

namespace Logging
{
    bool FilterRuleEvaluator::evaluate(const LogMessageHandle& log_message, const FilterRuleHandle& rule)
    {
        if (log_message->severity < rule->min_severity)
        {
            return false;
        }

        if (!containsPath(rule->path_pattern, log_message->source_path))
        {
            return false;
        }

        return true;
    }

    bool FilterRuleEvaluator::containsPath(const SourcePath& path_pattern, const SourcePath& path)
    {
        if (!path_pattern.is_wildcard && path_pattern.segments.size() != path.segments.size())
        {
            return false;
        }

        // iterate over pattern, may stop earlier that the path, but then it is a wildcard so it is fine
        for (uint32_t i = 0; i < path_pattern.segments.size(); i++)
        {
            if (path_pattern.segments[i] != path.segments[i])
            {
                return false;
            }
        }

        return true;
    }
} // Logging