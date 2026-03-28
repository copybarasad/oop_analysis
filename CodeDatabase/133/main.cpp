#include <exception>
#include <iostream>
#include "Game.h"

int main() {
	try {
		Game game;
		game.run();
	} catch (const std::exception& exception) {
		std::cerr << "Критическая ошибка: " << exception.what() << std::endl;
		return 1;
	}
	return 0;
}