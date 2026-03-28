#include "console_renderer.h"
#include "Game.h"
#include "Field.h"
#include "Node.h"
#include <iostream>

void ConsoleRenderer::render(const Game& game) {
    const Field* fieldPtr = game.getField();
    if (!fieldPtr) {
        std::cout << "Поле не инициализировано!" << std::endl;
        return;
    }
    const Field& field = *fieldPtr;
    
    const Player* player = game.getPlayer();
    if (!player) {
        std::cout << "Игрок не инициализирован!" << std::endl;
        return;
    }
    
    std::cout << "\n=== УРОВЕНЬ " << game.getCurrentLevel() << " ===" << std::endl;
    
    for (int y = 0; y < field.getLength(); ++y) {
        for (int x = 0; x < field.getWidth(); ++x) {
            switch (field.getNode(x, y).getType()) {
                case Type::EMPTY: std::cout << ". "; break;
                case Type::OBSTACLE: std::cout << "# "; break;
                case Type::PLAYER: std::cout << "P "; break;
                case Type::ENEMY: std::cout << "E "; break;
                case Type::TRAP: std::cout << "T "; break;
            }
        }
        std::cout << std::endl;
    }
    
    std::cout << std::endl;
    std::cout << "Игрок " << player->getName() 
              << " | HP: " << player->getLives() << "/" << player->getMaxLives() 
              << " | Урон: " << player->getCombatPower()
              << " | Очки: " << player->getPoints() << std::endl;
    
    for (const auto& enemy : game.getEnemies()) {
        if (enemy.isAlive()) {
            std::cout << "Враг " << enemy.getName() << " (" << enemy.getX() << ", " << enemy.getY() 
                      << ") | HP: " << enemy.getLives() << std::endl;
        }
    }
    
    player->getHand().printHand();
    std::cout << "=============================" << std::endl;
}