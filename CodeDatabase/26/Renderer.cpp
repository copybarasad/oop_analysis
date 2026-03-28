#include "Renderer.hpp"
#include "Cell.hpp"
#include "Field.hpp"
#include "GameWorld.hpp"
#include "Player.hpp"
#include "Position.hpp"
#include "SpellBase.hpp"
#include "SpellHand.hpp"
#include "SpellLibrary.hpp"
#include "ControlScheme.hpp"
#include "GameCommand.hpp"

#include <cstdlib>
#include <sstream>
#include <string>
#include <vector>
#include <map>

#ifdef _WIN32
#include <windows.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#ifndef DISABLE_NEWLINE_AUTO_RETURN
#define DISABLE_NEWLINE_AUTO_RETURN 0x0008
#endif
#endif

Renderer::Renderer(Theme theme) : theme_(theme) {}

void Renderer::initConsoleOnce() {
#if defined(_WIN32)
	static bool initialised = false;
	if (initialised) {
		return;
	}
	initialised = true;
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	if (handle == INVALID_HANDLE_VALUE) {
		return;
	}
	DWORD mode = 0;
	if (!GetConsoleMode(handle, &mode)) {
		return;
	}
	mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN;
	SetConsoleMode(handle, mode);
#endif
}

bool Renderer::shouldUseAsciiFallback() {
#if defined(_WIN32)
	return GetConsoleOutputCP() != 65001;
#else
	const char* term = std::getenv("TERM");
	if (!term) {
		return false;
	}
	return std::string(term).find("dumb") != std::string::npos;
#endif
}

std::string Renderer::fg(int r, int g, int b) {
	std::ostringstream out;
	out << "\x1b[38;2;" << r << ';' << g << ';' << b << 'm';
	return out.str();
}

std::string Renderer::reset() {
	return "\x1b[0m";
}

char Renderer::wallAscii() const noexcept { return '#'; }
char Renderer::slowAscii() const noexcept { return '~'; }
char Renderer::floorAscii() const noexcept { return '.'; }
char Renderer::playerAscii() const noexcept { return 'P'; }
char Renderer::enemyAscii() const noexcept { return 'E'; }
char Renderer::nestAscii() const noexcept { return 'N'; }
char Renderer::towerAscii() const noexcept { return 'T'; }
char Renderer::allyAscii() const noexcept { return 'A'; }
char Renderer::trapAscii() const noexcept { return '^'; }

std::string Renderer::wallFancy()   const { return fg(105,105,105) + u8"█" + reset(); }
std::string Renderer::slowFancy()   const { return fg(120,190,255) + u8"≈" + reset(); }
std::string Renderer::floorFancy()  const { return fg(80, 80, 80) + u8"·" + reset(); }
std::string Renderer::playerFancy() const { return fg(50,220,120) + u8"●" + reset(); }
std::string Renderer::enemyFancy()  const { return fg(220,20,60) + u8"●" + reset(); }
std::string Renderer::nestFancy()   const { return fg(255,200, 50) + u8"⌂" + reset(); }
std::string Renderer::towerFancy()  const { return fg(220,130,255) + u8"▲" + reset(); }
std::string Renderer::allyFancy()   const { return fg(80,160,255) + u8"●" + reset(); }
std::string Renderer::trapFancy()   const { return fg(255,140,40) + "*" + reset(); }

void Renderer::draw(const Field& field, const GameWorld& world, std::ostream& os) const {
	const bool fancy = (theme_ == Theme::Fancy);

#if defined(_WIN32)
	if (fancy) {
		os << "\x1b[?25l\x1b[H\x1b[2J\x1b[3J";
	} else {
		std::system("cls");
	}
#else
	if (fancy) {
		os << "\x1b[?25l";
	}
	os << "\x1b[H\x1b[2J";
#endif

	const int width = field.width();
	const int height = field.height();

	auto chooseAscii = [&](char symbol) -> char {
		switch (symbol) {
		case '#': return wallAscii();
		case '~': return slowAscii();
		case 'P': return playerAscii();
		case 'E': return enemyAscii();
		case 'N': return nestAscii();
		case 'T': return towerAscii();
		case 'A': return allyAscii();
		case '^': return trapAscii();
		default:  return floorAscii();
		}
	};

	auto chooseFancy = [&](char symbol) -> std::string {
		switch (symbol) {
		case '#': return wallFancy();
		case '~': return slowFancy();
		case 'P': return playerFancy();
		case 'E': return enemyFancy();
		case 'N': return nestFancy();
		case 'T': return towerFancy();
		case 'A': return allyFancy();
		case '^': return trapFancy();
		default:  return floorFancy();
		}
	};

	const int stacks = world.powerState().pendingStacks();
	std::vector<std::string> sidePanel;
	sidePanel.reserve(48);

	sidePanel.push_back("Level " + std::to_string(world.levelNumber()) + "/" + std::to_string(world.totalLevels()));
	sidePanel.push_back("Player stats:");
	const Player& playerRef = field.player();
	sidePanel.push_back("  HP " + std::to_string(playerRef.maxHealth()) +
	                    ", melee dmg " + std::to_string(playerRef.meleeDamage()));
	sidePanel.push_back("  Ranged dmg " + std::to_string(playerRef.rangedDamage()) +
	                    ", range " + std::to_string(playerRef.rangedRange()));
	sidePanel.push_back("Enemy stats:");
	sidePanel.push_back("  Spawn HP " + std::to_string(field.enemySpawnHp()) +
	                    ", dmg " + std::to_string(field.enemySpawnDamage()));
	const auto summonBase = SpellLibrary::summonStats();
	sidePanel.push_back("Ally stats:");
	sidePanel.push_back("  HP " + std::to_string(summonBase.hp) +
	                    ", dmg " + std::to_string(summonBase.damage) +
	                    " (+2 HP/+1 dmg per level)");
	sidePanel.push_back("");

	const auto& cards = world.hand().cards();
	sidePanel.push_back("Hand " + std::to_string(cards.size()) + "/" + std::to_string(world.hand().capacity()) + ":");
	std::map<int, std::pair<std::string, int>> handSummary;
	for (const auto& card : cards) {
		if (!card) {
			continue;
		}
		auto& entry = handSummary[card->hotkey()];
		entry.first = card->name();
		entry.second += 1;
	}
	if (handSummary.empty()) {
		sidePanel.push_back("  (empty)");
	} else {
		for (const auto& [hotkey, info] : handSummary) {
			std::string line = "  [" + std::to_string(hotkey) + "] " + info.first;
			if (info.second > 1) {
				line += " x" + std::to_string(info.second);
			}
			sidePanel.push_back(line);
		}
	}
	sidePanel.push_back("");

	const int baseDirectDamage = SpellLibrary::directBaseDamage();
	const int baseDirectRange = SpellLibrary::directBaseRange();
	const int effectiveDirectRange = baseDirectRange + stacks;

	const int baseAreaDamage = SpellLibrary::areaBaseDamage();
	const int baseAreaSize = SpellLibrary::areaBaseSize();
	const int baseAreaRange = SpellLibrary::areaBaseRange();
	const int effectiveAreaSize = baseAreaSize + stacks;

	const int baseTrapDamage = SpellLibrary::trapBaseDamage();
	const int trapRange = SpellLibrary::trapBaseRange();
	const int trapLifetime = SpellLibrary::trapBaseLifetime();
	const int effectiveTrapDamage = baseTrapDamage + stacks;

	const int effectiveSummonCount = summonBase.count + stacks;

	sidePanel.push_back("Spell reference:");
	sidePanel.push_back(" [1] Direct: dmg " + std::to_string(baseDirectDamage) +
	                    ", range " + std::to_string(effectiveDirectRange) + " (" +
	                    std::to_string(baseDirectRange) + "+stacks)");
	sidePanel.push_back(" [2] Area: dmg " + std::to_string(baseAreaDamage) +
	                    ", size " + std::to_string(effectiveAreaSize) + "x" +
	                    std::to_string(effectiveAreaSize) + " (" +
	                    std::to_string(baseAreaSize) + "+stacks), range " +
	                    std::to_string(baseAreaRange));
	sidePanel.push_back(" [3] Trap: dmg " + std::to_string(effectiveTrapDamage) +
	                    " (" + std::to_string(baseTrapDamage) + "+stacks), life " +
	                    std::to_string(trapLifetime) + ", range " + std::to_string(trapRange));
	sidePanel.push_back(" [4] Summon: hp " + std::to_string(summonBase.hp) +
	                    ", dmg " + std::to_string(summonBase.damage) +
	                    ", count " + std::to_string(effectiveSummonCount) +
	                    " (" + std::to_string(summonBase.count) + "+stacks)");
	sidePanel.push_back(" [5] Empower: +1 stack (boosts spell effects)");
	sidePanel.push_back("");
	sidePanel.push_back("Structures:");
	sidePanel.push_back(" Nest HP 20 (direct/area spells)");
	sidePanel.push_back(" Tower r4 dmg3 cd2 hp12");
	sidePanel.push_back("");
	sidePanel.push_back("Level overview:");
	for (const auto& info : world.levelPlan()) {
		std::string line = " L" + std::to_string(info.levelNumber) +
		                   " map " + std::to_string(info.width) + "x" + std::to_string(info.height) +
		                   " enemy " + std::to_string(info.enemyHp) + "/" + std::to_string(info.enemyDamage);
		if (info.playerHp.has_value() && info.playerMelee.has_value() && info.playerRanged.has_value()) {
			line += " player " + std::to_string(*info.playerHp) + "HP " +
			        std::to_string(*info.playerMelee) + "/" + std::to_string(*info.playerRanged);
		} else {
			line += " player ???";
		}
		sidePanel.push_back(line);
	}
	sidePanel.push_back("  (player stats unlock after reaching the level)");
	sidePanel.push_back("");
	sidePanel.push_back("Controls:");
	const ControlScheme* scheme = world.controlScheme();
	auto key = [&](GameCommand::Type type, char fallback) -> std::string {
		if (!scheme) {
			return std::string(1, fallback);
		}
		int code = scheme->keyFor(type);
		if (code <= 0) {
			return std::string(1, fallback);
		}
		return std::string(1, static_cast<char>(code));
	};
	sidePanel.push_back(" Move:        " + key(GameCommand::Type::MoveUp, 'w') + " " +
	                    key(GameCommand::Type::MoveDown, 's') + " " +
	                    key(GameCommand::Type::MoveLeft, 'a') + " " +
	                    key(GameCommand::Type::MoveRight, 'd'));
	sidePanel.push_back(" Attack:      " + key(GameCommand::Type::AttackUp, 'i') + " " +
	                    key(GameCommand::Type::AttackDown, 'k') + " " +
	                    key(GameCommand::Type::AttackLeft, 'j') + " " +
	                    key(GameCommand::Type::AttackRight, 'l'));
	sidePanel.push_back(" Switch mode: " + key(GameCommand::Type::SwitchMode, 'm'));
	sidePanel.push_back(" Cast spell:  " +
	                    key(GameCommand::Type::CastSpell1, '1') + " " +
	                    key(GameCommand::Type::CastSpell2, '2') + " " +
	                    key(GameCommand::Type::CastSpell3, '3') + " " +
	                    key(GameCommand::Type::CastSpell4, '4') + " " +
	                    key(GameCommand::Type::CastSpell5, '5'));
	sidePanel.push_back(" Save/Load:   " + key(GameCommand::Type::SaveGame, 'g') +
	                    " / " + key(GameCommand::Type::LoadGame, 'h'));
	sidePanel.push_back(" Quit:        " + key(GameCommand::Type::QuitToMenu, 'q'));

	os << "   ";
	for (int x = 0; x < width; ++x) {
		os << static_cast<char>('0' + (x % 10));
	}
	os << "   ";
	if (!sidePanel.empty()) {
		os << sidePanel[0];
	}
	os << '\n';

	std::size_t sideIndex = 1;
	for (int y = 0; y < height; ++y) {
		if (y < 10) {
			os << ' ' << static_cast<char>('0' + y) << ' ';
		} else {
			int hi = (y / 10) % 10;
			int lo = y % 10;
			os << static_cast<char>('0' + hi) << static_cast<char>('0' + lo) << ' ';
		}
		for (int x = 0; x < width; ++x) {
			const char symbol = field.symbolAt(Position{x, y});
			if (fancy) {
				os << chooseFancy(symbol);
			} else {
				os << chooseAscii(symbol);
			}
		}
		os << "   ";
		if (sideIndex < sidePanel.size()) {
			os << sidePanel[sideIndex];
		}
		++sideIndex;
		os << '\n';
	}

	while (sideIndex < sidePanel.size()) {
		os << "   ";
		for (int i = 0; i < width; ++i) {
			os << ' ';
		}
		os << "   " << sidePanel[sideIndex] << '\n';
		++sideIndex;
	}

	auto colourText = [&](int r, int g, int b, const std::string& text) -> std::string {
		if (!fancy) {
			return text;
		}
		return fg(r, g, b) + text + reset();
	};

	const int hpCurrent = field.player().currentHealth();
	const int hpMax = field.player().maxHealth();
	const int lastDamage = field.lastPlayerDamage();

	int hpR = 60;
	int hpG = 220;
	int hpB = 120;
	if (hpMax > 0) {
		double ratio = static_cast<double>(hpCurrent) / hpMax;
		if (ratio < 0.3) {
			hpR = 255; hpG = 80; hpB = 80;
		} else if (ratio < 0.6) {
			hpR = 255; hpG = 220; hpB = 50;
		}
	}

	os << colourText(hpR, hpG, hpB, "HP") << ": "
	   << colourText(hpR, hpG, hpB, std::to_string(hpCurrent))
	   << "/"
	   << colourText(hpR, hpG, hpB, std::to_string(hpMax));
	if (lastDamage > 0) {
		os << " " << colourText(255, 80, 80, "-" + std::to_string(lastDamage));
	}

	const std::string modeText =
	    (field.player().mode() == Player::CombatMode::Melee) ? "Melee" : "Ranged";
	os << "   "
	   << colourText(120, 190, 255, "Mode: ") << modeText;

	os << "   "
	   << colourText(150, 150, 150, "Enemies: ")
	   << std::to_string(field.enemyCount());

	os << "   "
	   << colourText(255, 80, 80, "Kills: ")
	   << std::to_string(field.player().kills());

	os << "   "
	   << colourText(150, 150, 150, "Empower stacks: ")
	   << std::to_string(stacks)
	   << '\n';

	if (fancy) {
		os << "\x1b[?25h";
	}
	os << std::flush;
}
