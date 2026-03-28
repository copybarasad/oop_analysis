#include "GameMenu.h"
#include <iostream>
#include <limits>

void GameMenu::clearInputBuffer() {
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

char GameMenu::getMenuChoice() {
	char choice;
	std::cin >> choice;
	clearInputBuffer();
	return choice;
}

void GameMenu::displayPauseOptions() {
	std::cout << "\n╔═════════════════════════════════════════╗\n";
	std::cout << "║            ИГРА НА ПАУЗЕ               ║\n";
	std::cout << "╚═════════════════════════════════════════╝\n";
	std::cout << "\n1. Продолжить игру\n";
	std::cout << "2. Сохранить игру\n";
	std::cout << "3. Загрузить игру\n";
	std::cout << "4. Выйти в главное меню\n";
	std::cout << "\nВаш выбор: ";
}

void GameMenu::displayStartOptions() {
	std::cout << "\n╔═════════════════════════════════════════╗\n";
	std::cout << "║           ДОБРО ПОЖАЛОВАТЬ!            ║\n";
	std::cout << "╚═════════════════════════════════════════╝\n";
	std::cout << "\n1. Начать новую игру\n";
	std::cout << "2. Загрузить игру\n";
	std::cout << "3. Выход\n";
	std::cout << "\nВаш выбор: ";
}

void GameMenu::displayGameOverOptions() {
	std::cout << "\n╔═════════════════════════════════════════╗\n";
	std::cout << "║            ИГРА ОКОНЧЕНА!              ║\n";
	std::cout << "╚═════════════════════════════════════════╝\n";
	std::cout << "\n1. Начать новую игру\n";
	std::cout << "2. Выход\n";
	std::cout << "\nВаш выбор: ";
}

MenuAction GameMenu::showPauseMenu() {
	displayPauseOptions();
	char choice = getMenuChoice();
	
	switch (choice) {
		case '1':
			return MenuAction::CONTINUE;
		case '2':
			return MenuAction::SAVE_GAME;
		case '3':
			return MenuAction::LOAD_GAME;
		case '4':
			return MenuAction::EXIT_GAME;
		default:
			std::cout << "\nНеверный выбор. Продолжаем игру...\n";
			return MenuAction::CONTINUE;
	}
}

MenuAction GameMenu::showStartMenu() {
	displayStartOptions();
	char choice = getMenuChoice();
	
	switch (choice) {
		case '1':
			return MenuAction::START_NEW_GAME;
		case '2':
			return MenuAction::LOAD_GAME;
		case '3':
			return MenuAction::EXIT_GAME;
		default:
			std::cout << "\nНеверный выбор. Начинаем новую игру...\n";
			return MenuAction::START_NEW_GAME;
	}
}

MenuAction GameMenu::showGameOverMenu() {
	displayGameOverOptions();
	char choice = getMenuChoice();
	
	switch (choice) {
		case '1':
			return MenuAction::START_NEW_GAME;
		case '2':
			return MenuAction::EXIT_GAME;
		default:
			std::cout << "\nНеверный выбор. Выходим...\n";
			return MenuAction::EXIT_GAME;
	}
}
