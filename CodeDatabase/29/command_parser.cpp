#include "command_parser.hpp"
#include <cctype>
#include <sstream>

CommandParser::CommandParser() = default;

bool CommandParser::LoadKeymap(const std::string& path, std::string& error) {
	return keymap_.LoadFromFile(path, error);
}

UserCommand CommandParser::Parse(const std::string& line) const {
	UserCommand cmd;
	cmd.raw = line;
	std::size_t i = 0;
	while (i < line.size() && std::isspace(static_cast<unsigned char>(line[i]))) ++i;
	if (i >= line.size()) return cmd;
	char key = line[i];
	cmd.type = keymap_.Resolve(key);
	if (cmd.type == UserCommandType::Unknown) return cmd;
	if (cmd.type == UserCommandType::Cast) {
		std::istringstream iss(line.substr(i + 1));
		iss >> cmd.slot >> cmd.targetX >> cmd.targetY;
	}
	return cmd;
}
