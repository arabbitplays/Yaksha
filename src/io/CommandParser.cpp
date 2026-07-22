#include "include/io/CommandParser.hpp"
#include "util/ShellUtil.hpp"
#include <charconv>
#include <exception>
#include <iostream>
#include <cassert>
#include <stdexcept>
#include <system_error>

namespace io {
    std::vector<std::string> splitAt(const std::string& input, char sep) {
        std::vector<std::string> result{};
        std::string curr_sub_str = "";
        for (char c : input) {
            if (c == sep || c == '\n') {
                if (curr_sub_str.empty())
                    continue;
                result.push_back(curr_sub_str);
                curr_sub_str.clear();
            } else {
                curr_sub_str += c;
            }
        }  
        return result;
    }

    std::shared_ptr<Command> CommandParser::parseCommand(const std::string& raw) {
        auto cmd = std::make_shared<Command>();
        std::vector<std::string> parts = splitAt(raw, ' ');
        if (parts.empty()) {
            throw std::runtime_error("Empty command");
        }
        cmd->keyword = parts.front();
        cmd->args.assign(parts.begin() + 1, parts.end());
        return cmd;
    }

    int32_t CommandParser::parseIntArg(const std::string& arg) {
        try {
            return ShellUtil::parseStringToInt(arg);
        } catch (std::exception& e) {
            throw std::runtime_error("Argument is not a valid integer!");
        }
    }
}
