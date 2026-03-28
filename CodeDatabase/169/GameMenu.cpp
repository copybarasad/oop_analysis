#include "GameMenu.hpp"
#include <iostream>
#include <limits>

void GameMenu::printMenu(bool hasSaveFile) {
	std::cout << "\n";
	std::cout << "главное меню\n";
	std::cout << " 1. новая игра\n";
	if (hasSaveFile) {
		std::cout << " 2. загрузить игру\n";
	}
	std::cout << " 3. выход\n";
	std::cout << "выберите действие: ";
}

void GameMenu::printCommands() {
	std::cout << "|| ваш ход:\n";
	std::cout << "|| w/s/a/d — движение\n";
	std::cout << "|| f — атака\n";
	std::cout << "|| t — смена режима\n";
	std::cout << "|| c <номер> <x> <y> — использовать заклинание\n";
	std::cout << "|| g — сохранить игру\n";
	std::cout << "|| l — загрузить игру\n";
	std::cout << "|| q — выход\n";
	std::cout << "|| введите команду: ";
}

char GameMenu::getMenuChoice() {
	char choice;
	std::cin >> choice;
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	return choice;
}

std::string GameMenu::getSaveFileName(const std::string& prompt, const std::string& defaultName) {
	std::string filename;
	std::cout << prompt;
	std::getline(std::cin, filename);
	if (filename.empty()) {
		return defaultName;
	}
	return filename;
}









