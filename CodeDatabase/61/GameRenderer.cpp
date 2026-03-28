#include "GameRenderer.h"
#include "Game.h"
#include "GameField.h"
#include "Cell.h"
#include "Player.h"
#include "ControlConfig.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>

GameRenderer::GameRenderer(std::ostream& output)
	: outputStream(&output) {
}

void GameRenderer::renderWelcomeMessage() const {
	if (!outputStream) return;
	
	*outputStream << "=== Welcome to the Adventure Game! ===" << std::endl;
	*outputStream << "Your goal is to survive and eliminate all enemies." << std::endl;
	*outputStream << "Buildings will spawn new enemies periodically." << std::endl;
	*outputStream << "Be careful of obstacles and slowing cells!" << std::endl;
	*outputStream << std::endl;
}

void GameRenderer::renderGameControls() const {
	if (!outputStream) return;
	
	// Используем значения по умолчанию для обратной совместимости
	*outputStream << "=== Game Controls ===" << std::endl;
	*outputStream << "w - Move up" << std::endl;
	*outputStream << "s - Move down" << std::endl;
	*outputStream << "a - Move left" << std::endl;
	*outputStream << "d - Move right" << std::endl;
	*outputStream << "f - Attack, then choose direction (w/a/s/d). Melee: 1 tile, Ranged: up to 3 tiles" << std::endl;
	*outputStream << "c - Cast spell (select spell and target)" << std::endl;
	*outputStream << "h - Show hand (spells)" << std::endl;
	*outputStream << "q - Switch combat mode (takes 1 turn)" << std::endl;
	*outputStream << "v - Save game" << std::endl;
	*outputStream << "z - Load game" << std::endl;
	*outputStream << "x - Exit game" << std::endl;
}

void GameRenderer::renderGameControls(const ControlConfig& config) const {
	if (!outputStream) return;
	
	*outputStream << "=== Game Controls ===" << std::endl;
	*outputStream << config.getKeyForCommand(ControlConfig::CommandType::MOVE_UP) << " - Move up" << std::endl;
	*outputStream << config.getKeyForCommand(ControlConfig::CommandType::MOVE_DOWN) << " - Move down" << std::endl;
	*outputStream << config.getKeyForCommand(ControlConfig::CommandType::MOVE_LEFT) << " - Move left" << std::endl;
	*outputStream << config.getKeyForCommand(ControlConfig::CommandType::MOVE_RIGHT) << " - Move right" << std::endl;
	
	char attackKey = config.getKeyForCommand(ControlConfig::CommandType::ATTACK);
	char moveUpKey = config.getKeyForCommand(ControlConfig::CommandType::MOVE_UP);
	char moveDownKey = config.getKeyForCommand(ControlConfig::CommandType::MOVE_DOWN);
	char moveLeftKey = config.getKeyForCommand(ControlConfig::CommandType::MOVE_LEFT);
	char moveRightKey = config.getKeyForCommand(ControlConfig::CommandType::MOVE_RIGHT);
	
	*outputStream << attackKey << " - Attack, then choose direction (" 
	              << moveUpKey << "/" << moveLeftKey << "/" << moveDownKey << "/" << moveRightKey 
	              << "). Melee: 1 tile, Ranged: up to 3 tiles" << std::endl;
	*outputStream << config.getKeyForCommand(ControlConfig::CommandType::CAST_SPELL) << " - Cast spell (select spell and target)" << std::endl;
	*outputStream << config.getKeyForCommand(ControlConfig::CommandType::SHOW_HAND) << " - Show hand (spells)" << std::endl;
	*outputStream << config.getKeyForCommand(ControlConfig::CommandType::SWITCH_MODE) << " - Switch combat mode (takes 1 turn)" << std::endl;
	*outputStream << config.getKeyForCommand(ControlConfig::CommandType::SAVE) << " - Save game" << std::endl;
	*outputStream << config.getKeyForCommand(ControlConfig::CommandType::LOAD) << " - Load game" << std::endl;
	*outputStream << config.getKeyForCommand(ControlConfig::CommandType::EXIT) << " - Exit game" << std::endl;
}

void GameRenderer::renderGameLegend() const {
	if (!outputStream) return;
	
	*outputStream << "\nLegend:" << std::endl;
	*outputStream << "P - Player" << std::endl;
	*outputStream << "E - Enemy" << std::endl;
	*outputStream << "B - Building" << std::endl;
	*outputStream << "A - Ally" << std::endl;
	*outputStream << "T - Trap" << std::endl;
	*outputStream << "# - Obstacle" << std::endl;
	*outputStream << "~ - Slowing cell" << std::endl;
	*outputStream << ". - Empty cell" << std::endl;
	*outputStream << "=====================" << std::endl;
}

void GameRenderer::renderGameStatus(const Game& game) const {
	if (!outputStream) return;
	
	// Проверяем, что игрок инициализирован (через проверку состояния игры)
	// Если игрок не создан, просто выводим сообщение
	try {
		const Player& player = game.getPlayer();
	int turnNumber = game.getTurnNumber();
	
	*outputStream << "\n=== Turn " << turnNumber << " ===" << std::endl;
	*outputStream << "Player Health: " << player.getHealth() << "/" << player.getMaxHealth() << std::endl;
	*outputStream << "Player Score: " << player.getScore() << std::endl;
	
	std::string combatModeStr = (player.getCombatMode() == CombatMode::MELEE) ? "Melee" : "Ranged";
	*outputStream << "Combat Mode: " << combatModeStr << std::endl;
	*outputStream << "Player Position: (" << player.getPositionX() << ", " << player.getPositionY() << ")" << std::endl;
	
	// Подсчитываем живых врагов
	const auto& enemies = game.getEnemies();
	int aliveEnemies = 0;
	for (const auto& enemy : enemies) {
		if (enemy.isAlive()) {
			aliveEnemies++;
		}
	}
	*outputStream << "Alive Enemies: " << aliveEnemies << std::endl;
	
	// Подсчитываем живых союзников
	const auto& allies = game.getAllies();
	int aliveAllies = 0;
	for (const auto& ally : allies) {
		if (ally.isAlive()) {
			aliveAllies++;
		}
	}
	*outputStream << "Alive Allies: " << aliveAllies << std::endl;
	
	if (player.getHand()) {
		*outputStream << "Spells in hand: " << player.getHand()->getSpellCount() << "/" 
		              << player.getHand()->getMaxSize() << std::endl;
	}
	} catch (const std::exception& e) {
		// Игрок еще не создан, выводим сообщение
		*outputStream << "\n=== Game not initialized ===" << std::endl;
		*outputStream << "Player not yet created. Call startLevel() first." << std::endl;
	}
}

void GameRenderer::renderGameField(const GameField& field) const {
	if (!outputStream) return;
	
	int width = field.getWidth();
	int height = field.getHeight();
	
	// Вычисляем ширину координат для правильного выравнивания
	int maxCoordinate = std::max(width - 1, height - 1);
	int coordinateWidth = calculateCoordinateWidth(maxCoordinate);
	
	// Отрисовываем заголовок с координатами колонок (X координаты)
	renderFieldHeader(width, coordinateWidth);
	
	// Отрисовываем поле с координатами строк (Y координаты)
	for (int y = 0; y < height; ++y) {
		renderFieldRow(field, y, coordinateWidth);
	}
}

void GameRenderer::renderFullGameState(const Game& game) const {
	renderGameStatus(game);
	if (!outputStream) return;
	
	*outputStream << "\nGame Field:" << std::endl;
	renderGameField(game.getField());
}

void GameRenderer::renderMessage(const std::string& message) const {
	if (!outputStream) return;
	*outputStream << message << std::endl;
}

void GameRenderer::renderLine(const std::string& line) const {
	if (!outputStream) return;
	*outputStream << line << std::endl;
}

char GameRenderer::getCellCharacter(const GameField& field, int x, int y) const {
	const Cell& cell = field.getCell(x, y);
	
	if (cell.hasPlayerOnCell()) {
		return 'P';
	} else if (cell.hasEnemyOnCell()) {
		return 'E';
	} else if (cell.hasBuildingOnCell()) {
		return 'B';
	} else if (cell.hasAllyOnCell()) {
		return 'A';
	} else if (cell.hasTrapOnCell()) {
		return 'T';
	} else {
		switch (cell.getType()) {
			case CellType::OBSTACLE:
				return '#';
			case CellType::SLOWING:
				return '~';
			case CellType::EMPTY:
			default:
				return '.';
		}
	}
}

int GameRenderer::calculateCoordinateWidth(int maxCoordinate) const {
	if (maxCoordinate < 10) {
		return 1;
	} else if (maxCoordinate < 100) {
		return 2;
	} else {
		return 3;
	}
}

void GameRenderer::renderFieldHeader(int width, int coordinateWidth) const {
	if (!outputStream) return;
	
	// Пробелы для выравнивания с координатами строк
	*outputStream << std::string(coordinateWidth + 1, ' ');
	
	// Координаты колонок
	for (int x = 0; x < width; ++x) {
		*outputStream << std::setw(coordinateWidth) << x << " ";
	}
	*outputStream << std::endl;
}

void GameRenderer::renderFieldRow(const GameField& field, int rowY, int coordinateWidth) const {
	if (!outputStream) return;
	
	// Координата строки слева
	*outputStream << std::setw(coordinateWidth) << rowY << " ";
	
	// Содержимое клеток
	int width = field.getWidth();
	for (int x = 0; x < width; ++x) {
		char cellChar = getCellCharacter(field, x, rowY);
		// Выравниваем символы клеток по ширине координат
		*outputStream << std::setw(coordinateWidth) << cellChar << " ";
	}
	*outputStream << std::endl;
}

void GameRenderer::renderSeparator() const {
	if (!outputStream) return;
	*outputStream << "=====================" << std::endl;
}

