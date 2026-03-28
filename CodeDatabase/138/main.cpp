#include "Game.h"
#include "GameExceptions.h"
#include <iostream>
#include <clocale>
#ifdef _WIN32
	#include <windows.h>
#endif

int main() {
	#ifdef _WIN32
		SetConsoleOutputCP(CP_UTF8);
		SetConsoleCP(CP_UTF8);
		std::setlocale(LC_ALL, "ru_RU.UTF-8");
	#endif
	
	try {
		Game game;
		game.Run();
	} catch (const GameException& error) {
		std::cerr << "Критическая ошибка игры: " << error.what() << std::endl;
		return 1;
	} catch (const std::exception& error) {
		std::cerr << "Неожиданная ошибка: " << error.what() << std::endl;
		return 1;
	} catch (...) {
		std::cerr << "Неизвестная ошибка" << std::endl;
		return 1;
	}
	
	return 0;
}
