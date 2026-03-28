#ifndef CLIINPUTHANDLER_H
#define CLIINPUTHANDLER_H

#include "command.h"
#include "CLIConfigurator.h"
#include <iostream>
#include <map>
#include <conio.h>

class CLIInputHandler
{
private:
	std::map<char, COMMAND_TYPE> commandMap;
public:
	CLIInputHandler();
	Command getCommand();
};

#endif