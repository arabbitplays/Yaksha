#ifndef DESKTOP_MANAGER_SOURCEPATH_HPP
#define DESKTOP_MANAGER_SOURCEPATH_HPP
#include <string>
#include <vector>

namespace Logging
{
    struct SourcePath
    {
        explicit SourcePath(const std::string& path)
        {
            segments.push_back(path);
        }
        std::vector<std::string> segments{};
    };
}

#endif //DESKTOP_MANAGER_SOURCEPATH_HPP