#include "renderer_interface.h"
#include <iostream>

class ConsoleRenderer : public IRenderer {
public:
    void render(GeneralManager& manager) override {
        Field& field = manager.getField();
        PlayerManager& playerMan = manager.getPlayerManager();
        EnemyManager& enemyMan = manager.getEnemyManager();
        auto playerPos = playerMan.getPlayerPosition();

        int width = field.getWidth();
        int height = field.getHeight();
        const Cell* cell;

        std::cout << "   ";
        for (int x = 0; x < width; ++x) {
            if (x >= 10) {
                std::cout << x / 10 << " ";
            } else {
                std::cout << "  ";
            }
        }
        std::cout << std::endl;

        std::cout << "    ";
        for (int x = 0; x < width; ++x) {
            std::cout << x % 10 << " ";
        }
        std::cout << std::endl;

        for (int y = 0; y < height; ++y) {
            if (y < 10){
                std::cout << y << " | ";
            }else{
                std::cout << y << "| ";
            }

            for (int x = 0; x < width; ++x) {
                cell = field.getCell(x,y); 
                if (playerPos.first == x && playerPos.second == y) {
                    std::cout << "P ";
                } else if (enemyMan.hasEnemyAt(x, y) && enemyMan.isEnemyAliveAt(x, y)) {
                    std::cout << "E ";
                } else if (manager.getTowerManager().hasTowerAt(x, y)) {
                    std::cout << "T ";
                } else if (cell->getType() == CellType::EMPTY) {
                    std::cout << ". ";
                } else if (cell->getType() == CellType::WALL){
                    std::cout << "# ";
                } else {
                    std::cout << "t ";
                }
            }
            std::cout << std::endl;
        }
    }
    
    void printStatus(GeneralManager& manager) override {
        PlayerManager& playerMan = manager.getPlayerManager();
        EnemyManager& enemyMan = manager.getEnemyManager();
        
        auto player = playerMan.getPlayer();
        if (player) {
            std::cout << "HP: " << player->getHealth() 
                      << " | Damage: " << player->getDamage()
                      << " | Points: " << player->getPoints() 
                      << " | Fight: " << (player->getFightType() ? "Melee" : "Ranged") 
                      << std::endl;
        }
        
        std::cout << "Enemies alive: " << enemyMan.getAliveEnemyPositions().size() << std::endl;
        std::cout << "-----------------------------" << std::endl;
    }
    
    void clearScreen() const override {
        system("clear");
    }
    
    void printMessage(const std::string& message) override{
        std::cout << message << std::endl;
    }

    void printStartMenu() override {
        std::cout << "=== WELCOME TO THE GAME ===" << std::endl;
        std::cout << "1. New Game" << std::endl;
        std::cout << "2. Load Game" << std::endl;
        std::cout << "3. Exit" << std::endl;
        std::cout << "Choose option: ";
    }

    void printLvlUpMenu() override{
        std::cout << "=== LEVEL UP! Choose upgrade ===" << std::endl;
        std::cout << "1. Increase Health (+20)" << std::endl;
        std::cout << "2. Increase Damage (+5)" << std::endl;
        std::cout << "3. Upgrade Random Spell" << std::endl;
        std::cout << "4. Skip upgrade" << std::endl;
    }
};