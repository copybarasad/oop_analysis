#ifndef  CLICONFIGURATOR_H
#define CLICONFIGURATOR_H 

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "command.h"

class CLIConfigurator
{
public:
	CLIConfigurator() = default;
	std::map<char, COMMAND_TYPE> initDefaultCommands();
	std::map<std::string, COMMAND_TYPE> initConvertionMap();
	void changeCommand(std::map<char, COMMAND_TYPE>& commandMap, COMMAND_TYPE option, char command);
	std::map<char, COMMAND_TYPE> mergeCFGs(std::map<char, COMMAND_TYPE> commandUserMap, 
													std::map<char, COMMAND_TYPE> defaultCommandMap);
	std::map<char, COMMAND_TYPE> readCFG();
};

#endif