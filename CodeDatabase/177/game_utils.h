#ifndef GAME_UTILS_H
#define GAME_UTILS_H

#include "game_controller.h"

//Утилитный класс для отображения состояния игры. Инкапсулирует функцию printGameState как метод класса.
class GameStatePrinter {
public:
	/**
	 * Вывести состояние игры в консоль
	 * @param game Объект игры для отображения
	 */
	void printGameState(const GameController& game) const;
};

#endif