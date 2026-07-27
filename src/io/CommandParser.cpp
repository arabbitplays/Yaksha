#include "include/io/CommandParser.hpp"
#include <exception>
#include <stdexcept>

#include "core/util/StringUtil.h"

namespace io {
    std::shared_ptr<Command> CommandParser::parseCommand(const std::string& raw) {
        auto cmd = std::make_shared<Command>();
        std::vector<std::string> parts = StringUtil::split(raw, ' ');
        if (parts.empty()) {
            throw std::runtime_error("Empty command");
        }
        cmd->keyword = parts.front();
        cmd->args.assign(parts.begin() + 1, parts.end());
        return cmd;
    }

    int32_t CommandParser::parseIntArg(const std::string& arg) {
        try {
            return std::stoi(arg);
        } catch (std::exception& e) {
            throw std::runtime_error("Argument is not a valid integer!");
        }
    }
}
