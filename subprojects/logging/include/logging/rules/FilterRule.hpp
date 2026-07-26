#ifndef DESKTOP_MANAGER_FILTERRULE_HPP
#define DESKTOP_MANAGER_FILTERRULE_HPP
#include <logging/util/SourcePathUtil.hpp>
#include <memory>

#include <logging/messages/Severity.hpp>
#include <logging/messages/SourcePath.hpp>

namespace Logging
{
    struct FilterRule
    {
        FilterRule(const std::string& path_pattern, const Severity min_severity)
            : path_pattern(SourcePathUtil::parseFromPathPattern(path_pattern)), min_severity(min_severity) {}
        SourcePath path_pattern;
        Severity min_severity;
    };

    typedef std::shared_ptr<FilterRule> FilterRuleHandle;
}

#endif //DESKTOP_MANAGER_FILTERRULE_HPP
