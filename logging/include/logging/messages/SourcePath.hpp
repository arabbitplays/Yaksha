#ifndef DESKTOP_MANAGER_SOURCEPATH_HPP
#define DESKTOP_MANAGER_SOURCEPATH_HPP

#include <string>
#include <vector>

namespace Logging
{
    struct SourcePath
    {
        explicit SourcePath(const std::vector<std::string>& segments, const bool is_wildcard = false)
            : segments(segments), is_wildcard(is_wildcard) { }
        std::vector<std::string> segments{};
        bool is_wildcard;
    };
}

#endif //DESKTOP_MANAGER_SOURCEPATH_HPP
