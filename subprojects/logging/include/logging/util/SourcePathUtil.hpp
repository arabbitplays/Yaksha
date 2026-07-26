#ifndef DESKTOP_MANAGER_SOURCEPATHUTIL_HPP
#define DESKTOP_MANAGER_SOURCEPATHUTIL_HPP

#include <cassert>
#include <logging/messages/SourcePath.hpp>
#include <cstdint>

namespace Logging
{
    class SourcePathUtil
    {
    public:
        static SourcePath parseFromClassId(const std::string& class_id)
        {
            std::vector<std::string> segments{};
            std::string trimmed_id = std::isdigit(class_id[0])
                ? class_id
                : class_id.substr(1, class_id.length() - 2);

            uint32_t curr_char_idx = 0;
            while (curr_char_idx < trimmed_id.length())
            {
                std::string length_string = "";
                while (std::isdigit(trimmed_id[curr_char_idx]))
                {
                    length_string += trimmed_id[curr_char_idx++];
                }

                uint32_t segment_len = stoi(length_string);
                std::string segment = trimmed_id.substr(curr_char_idx, segment_len);
                segments.push_back(segment);

                curr_char_idx += segment_len;
            }

            return SourcePath(segments);
        }

        static SourcePath parseFromPathPattern(const std::string& path_pattern)
        {
            std::vector<std::string> segments{};
            uint32_t start_idx = 0;
            for (uint32_t i = 0; i < path_pattern.length(); ++i)
            {
                if (path_pattern[i] == SEPARATOR_CHAR)
                {
                    std::string segment = path_pattern.substr(start_idx, i - start_idx);
                    if (!segment.empty())
                    {
                        segments.push_back(segment);
                    }
                    start_idx = i + 1;
                }
            }

            bool wildcard = false;
            if (path_pattern.empty() // empty pattern is wildcard
                || (path_pattern[path_pattern.length() - 1] == WILDCARD_CHAR
                    && (path_pattern.length() == 1 || path_pattern[path_pattern.length() - 2] == SEPARATOR_CHAR)))
            // only if the whole last segment is only the wildcard char
            {
                wildcard = true;
            }
            else if (start_idx < path_pattern.size())
            {
                segments.push_back(path_pattern.substr(start_idx));
            }

            return SourcePath{segments, wildcard};
        }

        static constexpr char WILDCARD_CHAR = '*';
        static constexpr char SEPARATOR_CHAR = '.';
    };
}
#endif //DESKTOP_MANAGER_SOURCEPATHUTIL_HPP
