#include "AoESpell.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyTower.h"
#include "GameGrid.h"
#include "MovementSystem.h"
#include <iostream>

AoESpell::AoESpell(int damage) : damage(damage) {}

bool AoESpell::cast(Player& player, Direction direction,
                    std::vector<std::unique_ptr<Enemy>>& enemies,
                    std::vector<std::unique_ptr<EnemyTower>>& towers,
                    GameGrid& grid) {
    int px = player.getPosition().first;
    int py = player.getPosition().second;

    int tx = px, ty = py;
    switch (direction) {
        case Direction::Up:    ty -= 2; break;
        case Direction::Down:  ty += 2; break;
        case Direction::Left:  tx -= 2; break;
        case Direction::Right: tx += 2; break;
    }

    if (!grid.isValidCoordinate(tx, ty)) {
        std::cout << "AoE center out of bounds (distance 2)." << std::endl;
        return false;
    }

    std::cout << "Casting AoE spell centered at (" << tx << "," << ty << ")" << std::endl;

    bool hit = false;
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            int cx = tx + dx;
            int cy = ty + dy;

            if (!grid.isValidCoordinate(cx, cy)) continue;

            for (auto& enemy : enemies) {
                if (enemy && enemy->getPosition().first == cx && enemy->getPosition().second == cy) {
                    enemy->takeDamage(damage);
                    hit = true;
                    if (!enemy->isAlive()) {
                        player.addScore(30);
                        grid.updateCell(cx, cy, CellType::Empty);
                    }
                }
            }

            for (auto& tower : towers) {
                if (tower && tower->getPosition().first == cx && tower->getPosition().second == cy) {
                    tower->takeDamage(damage);
                    hit = true;
                }
            }
        }
    }

    if (hit) {
        std::cout << "AoE spell hit targets!" << std::endl;
    } else {
        std::cout << "AoE spell cast, but no targets in area." << std::endl;
    }

    return true;
}