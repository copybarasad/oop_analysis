#include "spell.hpp"
#include "spell_direct.hpp"
#include "spell_aoe.hpp"
#include "spell_trap.hpp"
#include "spell_towerbolt.hpp"
#include <sstream>
#include <stdexcept>
#include <vector>

namespace {
	std::vector<std::string> SplitByColon(const std::string& code) {
		std::vector<std::string> parts;
		std::string current;
		for (char ch : code) {
			if (ch == ':') {
				parts.push_back(current);
				current.clear();
			} else {
				current.push_back(ch);
			}
		}
		parts.push_back(current);
		return parts;
	}

	int ParseInt(const std::string& text) {
		try {
			return std::stoi(text);
		} catch (const std::exception&) {
			throw std::invalid_argument("Invalid number in spell code");
		}
	}
}

std::unique_ptr<ISpell> ISpell::Deserialize(const std::string& code) {
	if (code.empty()) return nullptr;
	auto parts = SplitByColon(code);
	if (parts.empty()) return nullptr;
	const std::string& type = parts[0];
	auto need = [&](std::size_t count) {
		if (parts.size() != count) {
			throw std::invalid_argument("Invalid spell data");
		}
	};

	if (type == "DIRECT") {
		need(3);
		return std::unique_ptr<ISpell>(new DirectDamageSpell(ParseInt(parts[1]), ParseInt(parts[2])));
	}
	if (type == "AOE2X2") {
		need(3);
		return std::unique_ptr<ISpell>(new AoESpell2x2(ParseInt(parts[1]), ParseInt(parts[2])));
	}
	if (type == "TRAP") {
		need(3);
		return std::unique_ptr<ISpell>(new TrapSpell(ParseInt(parts[1]), ParseInt(parts[2])));
	}
	if (type == "TOWERBOLT") {
		need(3);
		return std::unique_ptr<ISpell>(new TowerBoltSpell(ParseInt(parts[1]), ParseInt(parts[2])));
	}

	throw std::invalid_argument("Unknown spell type: " + type);
}
