#include "DirectDamageSpell.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyTower.h"
#include "GameGrid.h"
#include "MovementSystem.h"
#include <iostream>

DirectDamageSpell::DirectDamageSpell(int damage) : damage(damage) {}

bool DirectDamageSpell::cast(Player& player, Direction direction,
                             std::vector<std::unique_ptr<Enemy>>& enemies,
                             std::vector<std::unique_ptr<EnemyTower>>& towers,
                             GameGrid& grid) {
    int px = player.getPosition().first;
    int py = player.getPosition().second;

    for (int dist = 1; dist <= 2; ++dist) {
        int tx = px, ty = py;
        switch (direction) {
            case Direction::Up:    ty -= dist; break;
            case Direction::Down:  ty += dist; break;
            case Direction::Left:  tx -= dist; break;
            case Direction::Right: tx += dist; break;
        }

        if (!grid.isValidCoordinate(tx, ty)) continue;

        for (auto& enemy : enemies) {
            if (enemy && enemy->getPosition().first == tx && enemy->getPosition().second == ty) {
                std::cout << "Direct Damage hits enemy at (" << tx << "," << ty << ") for " << damage << " damage!" << std::endl;
                enemy->takeDamage(damage);
                if (!enemy->isAlive()) {
                    player.addScore(50);
                    grid.updateCell(tx, ty, CellType::Empty);
                }
                return true;
            }
        }

        for (auto& tower : towers) {
            if (tower && tower->getPosition().first == tx && tower->getPosition().second == ty) {
                std::cout << "Direct Damage hits tower at (" << tx << "," << ty << ") for " << damage << " damage!" << std::endl;
                tower->takeDamage(damage);
                return true;
            }
        }
    }

    std::cout << "No target found for Direct Damage spell." << std::endl;
    return false;
}