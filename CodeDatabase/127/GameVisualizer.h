#ifndef GAMEVISUALIZER_H
#define GAMEVISUALIZER_H

#include "GameRenderer.h"
#include <memory>

class Game;

// Шаблонный класс визуализации игры
// RendererType - класс, отвечающий за способ отрисовки
template<typename RendererType>
class GameVisualizer {
private:
	Game* game_;  
	RendererType renderer_;  

public:
	GameVisualizer(Game* game, RendererType renderer)
		: game_(game), renderer_(std::move(renderer)) {}

	// Отрисовка текущего состояния игры
	void render() {
		if (game_ != nullptr) {
			renderer_.render(*game_);
		}
	}

	Game* getGame() const { return game_; }
};

#endif 





