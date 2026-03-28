#include "ConsoleInputReader.h"
#include "Command.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>

ConsoleInputReader::ConsoleInputReader(ControlConfig* controlConfig) 
	: controlConfig_(controlConfig) {
}

std::unique_ptr<Command> ConsoleInputReader::readCommand() {
	std::string input;
	if (!std::getline(std::cin, input)) {
		return std::make_unique<ExitCommand>();
	}

	// Удаляем пробелы в начале и конце
	input.erase(0, input.find_first_not_of(" \t\r\n"));
	input.erase(input.find_last_not_of(" \t\r\n") + 1);

	if (input.empty()) {
		return std::make_unique<NoCommand>();
	}

	return parseCommand(input);
}

std::unique_ptr<Command> ConsoleInputReader::parseCommand(const std::string& input) {
	if (input.empty()) {
		return std::make_unique<NoCommand>();
	}
	char firstChar = input[0];
	if (controlConfig_ != nullptr) {
		std::string command = controlConfig_->getCommandForKey(firstChar);
		if (!command.empty()) {
			if (command == "move_up") return std::make_unique<MoveUpCommand>();
			if (command == "move_down") return std::make_unique<MoveDownCommand>();
			if (command == "move_left") return std::make_unique<MoveLeftCommand>();
			if (command == "move_right") return std::make_unique<MoveRightCommand>();
			if (command == "switch_melee") return std::make_unique<SwitchMeleeCommand>();
			if (command == "switch_ranged") return std::make_unique<SwitchRangedCommand>();
			if (command == "use_spell") {
				std::istringstream iss(input);
				std::string cmd;
				int index, tx, ty;
				if (iss >> cmd >> index >> tx >> ty) {
					return std::make_unique<UseSpellCommand>(index, tx, ty);
				}
				return std::make_unique<NoCommand>();
			}
			if (command == "buy_spell") return std::make_unique<BuySpellCommand>();
			if (command == "save") {
				std::istringstream iss(input);
				std::string cmd, filename;
				if (iss >> cmd >> filename) {
					return std::make_unique<SaveCommand>(filename);
				}
				return std::make_unique<SaveCommand>("game.sav");
			}
			if (command == "load") {
				std::istringstream iss(input);
				std::string cmd, filename;
				if (iss >> cmd >> filename) {
					return std::make_unique<LoadCommand>(filename);
				}
				return std::make_unique<NoCommand>();
			}
			if (command == "menu") return std::make_unique<MenuCommand>();
			if (command == "exit") return std::make_unique<ExitCommand>();
			if (command == "help") return std::make_unique<HelpCommand>();
			if (command == "attack") {
				char direction = '\0';
				std::istringstream iss(input);
				std::string cmd;
				std::string dirStr;
				if (iss >> cmd) {
					if (iss >> dirStr && !dirStr.empty()) {
						direction = dirStr[0];
					}
				}
				if (direction == '\0' && input.length() > 1) {
					for (size_t i = 1; i < input.length(); ++i) {
						if (input[i] != ' ' && input[i] != '\t' && input[i] != '\r' && input[i] != '\n') {
							direction = input[i];
							break;
						}
					}
				}
				
				if (direction != '\0' && controlConfig_ != nullptr) {
					std::string dirCommand = controlConfig_->getCommandForKey(direction);
					if (!dirCommand.empty()) {
						if (dirCommand == "move_up") direction = 'w';
						else if (dirCommand == "move_down") direction = 's';
						else if (dirCommand == "move_left") direction = 'a';
						else if (dirCommand == "move_right") direction = 'd';
						else {
							direction = '\0';
						}
					} else {
						direction = '\0';
					}
				}
				
				return std::make_unique<AttackCommand>(direction);
			}
		}
		if (input.length() >= 2 && (firstChar == 'f' || firstChar == 'F')) {
			char direction = '\0';
			for (size_t i = 1; i < input.length(); ++i) {
				if (input[i] != ' ' && input[i] != '\t' && input[i] != '\r' && input[i] != '\n') {
					direction = input[i];
					break;
				}
			}
			
			if (direction != '\0' && controlConfig_ != nullptr) {
				std::string dirCommand = controlConfig_->getCommandForKey(direction);
				if (!dirCommand.empty()) {
					if (dirCommand == "move_up") direction = 'w';
					else if (dirCommand == "move_down") direction = 's';
					else if (dirCommand == "move_left") direction = 'a';
					else if (dirCommand == "move_right") direction = 'd';
					else {
						direction = '\0';
					}
				} else {
					direction = '\0';
				}
			}
			
			return std::make_unique<AttackCommand>(direction);
		}
		
		return std::make_unique<NoCommand>();
	}

	std::istringstream iss(input);
	std::string cmd;
	iss >> cmd;

	std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);

	if (cmd == "w" || cmd == "W") return std::make_unique<MoveUpCommand>();
	if (cmd == "s" || cmd == "S") return std::make_unique<MoveDownCommand>();
	if (cmd == "a" || cmd == "A") return std::make_unique<MoveLeftCommand>();
	if (cmd == "d" || cmd == "D") return std::make_unique<MoveRightCommand>();
	if (cmd == "m" || cmd == "M") return std::make_unique<SwitchMeleeCommand>();
	if (cmd == "r" || cmd == "R") return std::make_unique<SwitchRangedCommand>();
	if (cmd == "b" || cmd == "B") return std::make_unique<BuySpellCommand>();
	if (cmd == "q" || cmd == "Q" || cmd == "menu") return std::make_unique<MenuCommand>();
	if (cmd == "x" || cmd == "X" || cmd == "exit") return std::make_unique<ExitCommand>();
	if (cmd == "h" || cmd == "H" || cmd == "help") return std::make_unique<HelpCommand>();

	if (cmd == "u" || cmd == "U") {
		int index, tx, ty;
		if (iss >> index >> tx >> ty) {
			return std::make_unique<UseSpellCommand>(index, tx, ty);
		}
		return std::make_unique<NoCommand>();
	}

	if (cmd == "f" || cmd == "F") {
		char direction = '\0';
		for (size_t i = cmd.length(); i < input.length(); ++i) {
			if (input[i] != ' ' && input[i] != '\t') {
				direction = input[i];
				break;
			}
		}
		if (direction != '\0' && controlConfig_ != nullptr) {
			std::string dirCommand = controlConfig_->getCommandForKey(direction);
			if (!dirCommand.empty()) {
				if (dirCommand == "move_up") direction = 'w';
				else if (dirCommand == "move_down") direction = 's';
				else if (dirCommand == "move_left") direction = 'a';
				else if (dirCommand == "move_right") direction = 'd';
			}
		}
		return std::make_unique<AttackCommand>(direction);
	}

	if (cmd == "save") {
		std::string filename;
		if (iss >> filename) {
			return std::make_unique<SaveCommand>(filename);
		}
		return std::make_unique<SaveCommand>("game.sav");
	}

	if (cmd == "load") {
		std::string filename;
		if (iss >> filename) {
			return std::make_unique<LoadCommand>(filename);
		}
		return std::make_unique<NoCommand>();
	}

	return std::make_unique<NoCommand>();
}

