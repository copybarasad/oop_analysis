#ifndef GAMELOOP_H
#define GAMELOOP_H

#include "GameManager.h"
#include "GameVisualizer.h"
#include "GameEventObserver.h"
#include <memory>

class Game;

template<typename InputReaderType, typename RendererType>
class GameLoop {
private:
	Game* game_;
	GameManager<InputReaderType> manager_;
	GameVisualizer<RendererType> visualizer_;
	GameEventObserver* observer_;  

public:
	GameLoop(Game* game, InputReaderType inputReader, RendererType renderer, GameEventObserver* observer = nullptr)
		: game_(game), manager_(game, std::move(inputReader)), visualizer_(game, std::move(renderer)), observer_(observer) {}

	bool run() {
		if (game_ == nullptr) return false;

		game_->initializeTurn();

		while (true) {
			if (game_->shouldExitProgram()) {
				return false;
			}
			if (game_->shouldExitMenu()) {
				return false;
			}
			if (!game_->isPlayerAlive()) {
				game_->handlePlayerDeath();
				if (game_->shouldExitProgram() || game_->shouldExitMenu()) {
					return false;
				}
				continue;
			}
			if (game_->isLevelCleared()) {
				game_->handleLevelCleared();
				if (game_->shouldExitProgram() || game_->shouldExitMenu()) {
					return false;
				}
				continue;
			}

			// Отрисовка состояния игры
			visualizer_.render();

			// Очищаем сообщение об ошибке после отрисовки
			game_->clearErrorMessage();

			// Обработка ввода через GameManager
			manager_.processInput();

			// Проверка условий завершения после обработки команды
			if (game_->shouldExitProgram() || game_->shouldExitMenu()) {
				return false;
			}

			game_->alliesTurn();
			game_->enemiesTurn();
			game_->incrementTurn();
		}

		return false;
	}
};

#endif

