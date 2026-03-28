#ifndef GAME_COMMAND_H
#define GAME_COMMAND_H

enum class CommandType {
	None,
	Move,
	SwitchMode,
	RangedAttack,
	CastSpell,
	BuySpell,
	ShowInfo,
	Save,
	Load,
	Quit,
	Help,
	Invalid
};

struct GameCommand {
	CommandType type = CommandType::None;
	int deltaX = 0;
	int deltaY = 0;
	int spellIndex = -1;
};

#endif

