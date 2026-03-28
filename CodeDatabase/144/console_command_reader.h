#ifndef CONSOLE_COMMAND_READER_H
#define CONSOLE_COMMAND_READER_H

#include "game_command.h"
#include "control_config.h"

// Класс, считывающий ввод пользователя и преобразующий ввод пользователя в объект команды.
class ConsoleCommandReader {
public:
	explicit ConsoleCommandReader(ControlConfig config = ControlConfig::createDefault());

	// Считывает команду из консоли и преобразует её в объект GameCommand
	GameCommand readCommand() const;
	
	// Преобразует символ ввода в объект GameCommand
	GameCommand commandFromChar(char input) const;

private:
	// Вспомогательные методы для команд, требующих дополнительного ввода
	GameCommand readRangedAttackCommand() const;
	GameCommand readCastCommand() const;

private:
	ControlConfig controls;
};

#endif

