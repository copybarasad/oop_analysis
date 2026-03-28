#pragma once
#include <string>

class GameMenu {
public:
	static void printMenu(bool hasSaveFile);
	static void printCommands();
	static char getMenuChoice();
	static std::string getSaveFileName(const std::string& prompt, const std::string& defaultName);
};









