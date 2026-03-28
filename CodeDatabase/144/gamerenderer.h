#ifndef GAME_RENDERER_H
#define GAME_RENDERER_H

class Game;

// Класс отрисовки игры.
class GameRenderer {
public:
	explicit GameRenderer(const Game& game);
	
	// Отображает игровое поле и статистику для указанного хода
	void displayField(int turnCounter) const;
	
	// Отображает статистику игры (HP, мана, счет и т.д.)
	void displayGameStats(int turnCounter) const;

private:
	const Game& game;
};

#endif