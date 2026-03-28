#ifndef CONSOLE_INPUTER
#define CONSOLE_INPUTER

#include <unordered_map>
#include <iostream>
#include <functional>

#include "Commands.h"
#include "Constants.h"
#include "GameExceptions.h"

class ConsoleInputer
{
	std::unordered_map<char, Command> commandsTable;
public:
	ConsoleInputer();
	void GetCommandFromUser(Command&);
	void GetStringFromUser(std::string&);
	void Get—oordinates(std::pair<int, int>&, std::pair<int, int>&, std::pair<int, int>&);
	void GetTargetNumber(int&, std::pair<int, int>&);
};

#endif