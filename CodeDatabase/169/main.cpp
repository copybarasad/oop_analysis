#include <iostream>
#include "Game.hpp"

int main() {
	try {
		Game game;
		game.run();
	} catch (const std::exception& ex) {
		std::cerr << "Критическая ошибка: " << ex.what() << '\n';
		return 1;
	}

	return 0;
}
