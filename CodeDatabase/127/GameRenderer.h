#ifndef GAMERENDERER_H
#define GAMERENDERER_H

class Game;

// Интерфейс для отрисовки игры
class GameRenderer {
public:
	virtual ~GameRenderer() = default;
	virtual void render(const Game& game) = 0;
};

#endif 




