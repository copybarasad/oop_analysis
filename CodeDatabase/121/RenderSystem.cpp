#include "RenderSystem.h"
#include <iostream>

RenderSystem::RenderSystem(std::shared_ptr<TextSystem> textSystem) 
    : textSystem(textSystem) {}

void RenderSystem::renderGameState(const LevelController& level, int turnCount) {
    // Рендерим игровое поле
    renderGameField(level);
    
    // Получаем игрока и отображаем информацию
    const auto& player = level.getPlayer();
    
    // Отображаем статус игры
    std::cout << textSystem->getGameStatus(level.getLevelNumber(), turnCount, player.getHealth(), player.getScore()) << std::endl;
    
    // Отображаем информацию о бое
    std::string weapon = (player.getBattleMode() == BattleMode::CLOSE) ? "меч" : "лук";
    std::cout << textSystem->getBattleInfo(weapon, player.getCurrentDamage(), player.getAttackRange()) << std::endl;
    
    // Отображаем легенду
    std::cout << textSystem->getLegend() << std::endl;
}

void RenderSystem::renderGameField(const LevelController& level) {
    const auto& field = level.getField();
    
    std::cout << "\n";
    for (int y = 0; y < field.getHeight(); y++) {
        for (int x = 0; x < field.getWidth(); x++) {
            Position pos(x, y);
            std::cout << getCellSymbol(level, pos) << " ";
        }
        std::cout << std::endl;
    }
}

char RenderSystem::getCellSymbol(const LevelController& level, const Position& pos) const {
    const auto& field = level.getField();
    const auto& player = level.getPlayer();
    
    // Проверяем позицию игрока
    if (player.getPosition() == pos) return 'P';
    
    // Проверяем врагов
    for (const auto& enemy : level.getEnemies()) {
        if (enemy.isAlive() && enemy.getPosition() == pos) return 'E';
    }
    
    // Проверяем здания
    for (const auto& building : level.getBuildings()) {
        if (building.isAlive() && building.getPosition() == pos) return 'B';
    }
    
    // Проверяем башни
    for (const auto& tower : level.getTowers()) {
        if (tower.isAlive() && tower.getPosition() == pos) return 'T';
    }
    
    // Проверяем тип клетки
    const Cell& cell = field.getCell(pos);
    switch (cell.getType()) {
        case CellType::BLOCKED: return '#';
        case CellType::SLOW: return '*';
        case CellType::JOCKER: return 'J';
        default: return '.';
    }
}

void RenderSystem::renderCommands() {
    std::cout << textSystem->getCommands() << std::endl;
}

void RenderSystem::renderMessage(const std::string& message) {
    std::cout << "> " << message << std::endl;
}

void RenderSystem::renderGameOver(bool victory, const Player& player) {
    std::cout << textSystem->getGameOver(victory, player.getScore()) << std::endl;
}