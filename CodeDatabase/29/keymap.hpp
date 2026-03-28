#ifndef KEYMAP_HPP
#define KEYMAP_HPP

#include <unordered_map>
#include <string>
#include "command.hpp"

class Keymap {
public:
	Keymap();
	bool LoadFromFile(const std::string& path, std::string& error);
	UserCommandType Resolve(char key) const;
	const std::unordered_map<char, UserCommandType>& Mapping() const noexcept { return mapping_; }
private:
	std::unordered_map<char, UserCommandType> mapping_;
	void SetDefault();
	bool Validate(const std::unordered_map<char, UserCommandType>& map) const;
};

#endif
