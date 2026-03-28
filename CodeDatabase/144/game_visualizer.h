#ifndef GAME_VISUALIZER_H
#define GAME_VISUALIZER_H

class Game;

// Шаблонный класс визуализации игры.
template <typename Renderer>
class GameVisualizer {
public:
	explicit GameVisualizer(Game& game)
		: renderer(game), gameRef(game) {
	}

	// Реагирует на начало нового хода - отрисовывает текущее состояние игры
	void renderTurn(int turnCounter) {
		renderer.displayField(turnCounter);
	}

	// Реагирует на изменение состояния игры (например, после загрузки, изменения уровня)
	void onGameStateChanged(int turnCounter) {
		renderer.displayField(turnCounter);
	}

	// Реагирует на завершение игры
	void onGameEnd(int turnCounter, int finalScore) {
		renderer.displayField(turnCounter);
	}

private:
	Renderer renderer;
	Game& gameRef;
};

#endif

