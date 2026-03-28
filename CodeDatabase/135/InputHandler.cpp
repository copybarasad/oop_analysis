#include "InputHandler.h"
#include <iostream>
#include <limits>

void InputHandler::waitForEnter() const {
	std::cout << "Нажмите Enter для продолжения...\n";
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cin.get();
}

void InputHandler::clearInputBuffer() const {
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

UserInput InputHandler::getUserChoice() const {
	std::cout << "\n[P] Меню паузы | [Enter] Продолжить\n";
	std::cout << "Ваш выбор: ";
	
	std::string input;
	std::getline(std::cin, input);
	
	if (input == "P" || input == "p" || input == "П" || input == "п") {
		return UserInput::PAUSE;
	}
	
	return UserInput::CONTINUE;
}
