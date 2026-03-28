#include "color_vis.h"
#include "Game.h"
#include "Field.h"
#include "Node.h"
#include <iostream>

void ColorfulRenderer::render(const Game& game) {
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
    
    std::cout << "\033[95m" << "\n=== УРОВЕНЬ " << game.getCurrentLevel() << " ===" << "\033[0m" << std::endl;
    
    for (int y = 0; y < field.getLength(); ++y) {
        for (int x = 0; x < field.getWidth(); ++x) {
            Type cellType = field.getNode(x, y).getType();
            
            if (cellType == Type::PLAYER) {
                std::cout << "\033[92m" << "P " << "\033[0m";
            } else if (cellType == Type::ENEMY) {
                std::cout << "\033[91m" << "E " << "\033[0m";
            } else if (cellType == Type::OBSTACLE) {
                std::cout << "\033[96m" << "# " << "\033[0m";
            } else if (cellType == Type::TRAP) {
                std::cout << "\033[93m" << "T " << "\033[0m";
            } else {
                std::cout << "\033[94m" << ". " << "\033[0m";
            }
        }
        std::cout << std::endl;
    }
    
    std::cout << std::endl;
    std::cout << "\033[92m" << "Игрок " << player->getName() << "\033[0m"
              << " | " << "\033[91m" << "HP: " << player->getLives() << "/" 
              << player->getMaxLives() << "\033[0m"
              << " | " << "\033[93m" << "Урон: " << player->getCombatPower() << "\033[0m"
              << " | " << "\033[94m" << "Очки: " << player->getPoints() << "\033[0m" << std::endl;
    
    for (const auto& enemy : game.getEnemies()) {
        if (enemy.isAlive()) {
            std::cout << "\033[91m" << "Враг " << enemy.getName() << " (" << enemy.getX() << ", " 
                      << enemy.getY() << ") | HP: " << enemy.getLives() << "\033[0m" << std::endl;
        }
    }
    
    player->getHand().printHand();
    std::cout << "\033[95m" << "=============================" << "\033[0m" << std::endl;
}