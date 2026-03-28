#include "DisplayManager.hpp"
#include "GameWorld.hpp"
#include "Grid.hpp"
#include <iostream>

void DisplayManager::showMap(GameWorld& world)
{
    const int width  = world.getGrid().getWidth();
    const int height = world.getGrid().getHeight();

    const Player& player = world.getPlayer();
    const MainEnemy& mainEnemy = world.getMainEnemy();

    const Position playerPos = player.getPos();
    const Position bossPos   = mainEnemy.getPos();

    Position towerPos{-1,-1};
    if (auto* tower = world.getConstruction()) {
        towerPos = tower->getPos();
    }

    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            bool printed = false;

            if (i == playerPos.x && j == playerPos.y) { std::cout << "P "; printed = true; }
            else if (i == bossPos.x && j == bossPos.y) { std::cout << "\033[1;31mM \033[0m"; printed = true; }
            else if (i == towerPos.x && j == towerPos.y) { std::cout << "\033[1;31mT \033[0m "; printed = true; }
            else {
                for (auto& enemyPtr : world.getEnemies()) {
                    const Position epos = enemyPtr->getPos();
                    if (i == epos.x && j == epos.y) { std::cout << "\033[1;31mE \033[0m "; printed = true; break; }
                }
            }

            if (!printed) {
                switch (world.getGrid().getCell(j, i)) {
                    case cellType::free:      std::cout << ". "; break;
                    case cellType::forbidden: std::cout << "# "; break;
                    case cellType::slowed:    std::cout << "~ "; break;
                }
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}
