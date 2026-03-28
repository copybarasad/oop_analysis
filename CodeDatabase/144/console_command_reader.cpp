#include "console_command_reader.h"
#include <cctype>
#include <iostream>
#include <limits>

ConsoleCommandReader::ConsoleCommandReader(ControlConfig config)
	: controls(std::move(config)) {}

// После считывания клавиши, считанный символ сразу обрабатывается,
// и далее работа проводится с сущностью, которая представляет команду (GameCommand)
GameCommand ConsoleCommandReader::readCommand() const {
	std::cout << "Enter command: ";
	char input;
	std::cin >> input;
	return commandFromChar(input);  // Сразу преобразуем символ в объект команды
}

GameCommand ConsoleCommandReader::commandFromChar(char input) const {
	const CommandBinding* binding = controls.findBinding(input);
	if (!binding) {
		return {CommandType::Invalid, 0, 0, -1};
	}

	// Команды, требующие дополнительного ввода, обрабатываются отдельно
	if (binding->requiresDirection) {
		return readRangedAttackCommand();
	}
	if (binding->requiresSpellIndex) {
		return readCastCommand();
	}

	GameCommand command;
	command.type = binding->type;
	command.deltaX = binding->deltaX;
	command.deltaY = binding->deltaY;
	command.spellIndex = -1;

	return command;
}

GameCommand ConsoleCommandReader::readRangedAttackCommand() const {
	GameCommand command;
	command.type = CommandType::Invalid;
	command.deltaX = 0;
	command.deltaY = 0;
	command.spellIndex = -1;

	// Используем клавиши движения из текущей конфигурации
	auto getKey = [this](CommandName name, char fallback) {
		auto k = controls.keyFor(name);
		return k ? *k : fallback;
	};
	char upKey = getKey(CommandName::MoveUp, 'w');
	char downKey = getKey(CommandName::MoveDown, 's');
	char leftKey = getKey(CommandName::MoveLeft, 'a');
	char rightKey = getKey(CommandName::MoveRight, 'd');

	std::cout << "Enter direction for ranged attack ("
	          << upKey << "/" << downKey << "/" << leftKey << "/" << rightKey << "): ";

	char direction;
	std::cin >> direction;
	direction = static_cast<char>(std::tolower(static_cast<unsigned char>(direction)));

	if (direction == static_cast<char>(std::tolower(static_cast<unsigned char>(upKey)))) {
		command.type = CommandType::RangedAttack;
		command.deltaY = -1;
	} else if (direction == static_cast<char>(std::tolower(static_cast<unsigned char>(downKey)))) {
		command.type = CommandType::RangedAttack;
		command.deltaY = 1;
	} else if (direction == static_cast<char>(std::tolower(static_cast<unsigned char>(leftKey)))) {
		command.type = CommandType::RangedAttack;
		command.deltaX = -1;
	} else if (direction == static_cast<char>(std::tolower(static_cast<unsigned char>(rightKey)))) {
		command.type = CommandType::RangedAttack;
		command.deltaX = 1;
	} else {
		std::cout << "Invalid direction!\n";
	}

	return command;
}

GameCommand ConsoleCommandReader::readCastCommand() const {
	GameCommand command;
	command.type = CommandType::CastSpell;
	command.deltaX = 0;
	command.deltaY = 0;
	command.spellIndex = -1; // фактический выбор делает Game после показа списка
	return command;
}

