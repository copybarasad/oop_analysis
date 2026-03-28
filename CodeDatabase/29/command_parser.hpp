#ifndef COMMAND_PARSER_HPP
#define COMMAND_PARSER_HPP

#include <string>
#include "command.hpp"
#include "keymap.hpp"

class CommandParser {
public:
	CommandParser();
	bool LoadKeymap(const std::string& path, std::string& error);
	UserCommand Parse(const std::string& line) const;
	const Keymap& Map() const noexcept { return keymap_; }
private:
	Keymap keymap_;
};

#endif
