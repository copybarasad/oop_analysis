#include "Renderer.h"
#include <iostream>

void Renderer::renderGameState(const GameState& gameState) {
    std::cout << "\n=== ИГРА ===" << std::endl;
    renderPlayerStats(gameState.getPlayer());
    std::cout << std::endl;
    renderGameField(gameState);
    std::cout << std::endl;

    gameState.getPlayer().getSpellHand().displaySpells();
    std::cout << std::endl;

    std::cout << "Управление:" << std::endl;
    std::cout << "  WASD - движение/ближняя атака (в режиме ближнего боя)" << std::endl;
    std::cout << "  F - дальняя атака (автоматически по ближайшему врагу)" << std::endl;
    std::cout << "  SPACE - сменить режим боя" << std::endl;
    std::cout << "  G - сохранить игру" << std::endl;
    std::cout << "  Q - выход" << std::endl;
    std::cout << "Текущая дальность атаки: " << gameState.getPlayer().getAttackRange() << " клеток" << std::endl;
    
}


void Renderer::renderPlayerStats(const Player& player) {
    std::cout << "Здоровье: " << player.getHealth() << " | ";
    std::cout << "Мана: " << player.getMana() << " | ";
    std::cout << "Очки: " << player.getScore() << " | ";
    std::cout << "Режим боя: " << player.getCombatModeString() << " | ";
    std::cout << "Урон: " << player.getDamage() << std::endl;
}

void Renderer::renderGameField(const GameState& gameState) {
    for (int y = 0; y < gameState.getGameField().getHeight(); ++y) {
        for (int x = 0; x < gameState.getGameField().getWidth(); ++x) {
            std::cout << getCellSymbol(gameState, x, y) << ' ';
        }
        std::cout << std::endl;
    }
}

char Renderer::getCellSymbol(const GameState& gameState, int x, int y) {
    Position position(x, y);
    
    if (gameState.getPlayer().getPosition() == position) {
        return 'P';
    }
    
    for (const auto& enemy : gameState.getEnemies()) {
        if (enemy.isAlive() && enemy.getPosition() == position) {
            return 'E';
        }
    }
    
    return gameState.getGameField().getCell(x, y).getSymbol();
}

void Renderer::renderGameOverScreen(const GameState& gameState) {
    std::cout << "\n=== ИГРА ОКОНЧЕНА ===" << std::endl;
    if (gameState.getPlayer().isAlive()) {
        std::cout << "Поздравляем! Вы выжили!" << std::endl;
    } else {
        std::cout << "Вы погибли в бою..." << std::endl;
    }
    std::cout << "Итоговый счет: " << gameState.getPlayer().getScore() << std::endl;
}