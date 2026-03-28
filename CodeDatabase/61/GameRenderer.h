#ifndef GAME_RENDERER_H
#define GAME_RENDERER_H

#include <ostream>
#include <iostream>
#include <string>

// Forward declarations
class Game;
class GameField;
class Player;
class ControlConfig;

// Класс отрисовки игры - определяет, как должна отображаться игра
class GameRenderer {
protected:
	std::ostream* outputStream;  // protected для доступа из наследников
	
	// Вспомогательные методы для отрисовки элементов
	char getCellCharacter(const GameField& field, int x, int y) const;
	int calculateCoordinateWidth(int maxCoordinate) const;
	void renderFieldHeader(int width, int coordinateWidth) const;
	void renderFieldRow(const GameField& field, int rowY, int coordinateWidth) const;
	void renderSeparator() const;
	
public:
	GameRenderer(std::ostream& output = std::cout);
	
	// Основные методы отрисовки
	void renderWelcomeMessage() const;
	void renderGameControls() const;  // Старый метод (для совместимости)
	void renderGameControls(const ControlConfig& config) const;  // Новый метод с конфигурацией
	void renderGameLegend() const;
	void renderGameStatus(const Game& game) const;
	void renderGameField(const GameField& field) const;
	void renderFullGameState(const Game& game) const;
	
	// Вспомогательные методы
	void renderMessage(const std::string& message) const;
	void renderLine(const std::string& line) const;
};

#endif // GAME_RENDERER_H

