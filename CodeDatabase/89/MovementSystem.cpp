#include "MovementSystem.h"
#include <iostream>
#include <random>
#include <algorithm>

MovementSystem::MovementSystem(GameGrid& grid) : grid(grid) {}

bool MovementSystem::movePlayer(Player& player, Direction direction, 
                                const std::vector<std::unique_ptr<Enemy>>& enemies) {
    if (!player.isAlive()) return false;

    int currentX = player.getPosition().first;
    int currentY = player.getPosition().second;
    int newX = currentX;
    int newY = currentY;

    switch (direction) {
        case Direction::Up:    newY--; break;
        case Direction::Down:  newY++; break;
        case Direction::Left:  newX--; break;
        case Direction::Right: newX++; break;
    }

    if (!isValidMove(newX, newY)) {
        std::cout << "Cannot move to (" << newX << ", " << newY << ") - invalid or impassable." << std::endl;
        return false;
    }

    if (grid.getCell(newX, newY).getType() == CellType::Trap) {
        std::cout << "You cannot step on a trap!" << std::endl;
        return false;
    }

    if (isPositionOccupiedByEnemy(newX, newY, enemies)) {
        std::cout << "Player encountered an enemy at (" << newX << ", " << newY << "). Cannot move." << std::endl;
        return false;
    }
    
    updatePosition(currentX, currentY, newX, newY, CellType::Player);
    player.setPosition(newX, newY);
    std::cout << "Player moved to (" << newX << ", " << newY << ")." << std::endl;
    return true;
}

void MovementSystem::moveEnemies(std::vector<std::unique_ptr<Enemy>>& enemies, Player& player) {
    if (!player.isAlive()) return;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disDir(0, 3);

    for (auto& enemy : enemies) {
        if (!enemy || !enemy->isAlive()) continue;

        int currentX = enemy->getPosition().first;
        int currentY = enemy->getPosition().second;
        int newX = currentX;
        int newY = currentY;
        
        Direction direction = static_cast<Direction>(disDir(gen));

        switch (direction) {
            case Direction::Up:    newY--; break;
            case Direction::Down:  newY++; break;
            case Direction::Left:  newX--; break;
            case Direction::Right: newX++; break;
        }

        if (!isValidMove(newX, newY)) {
            continue;
        }

        if (player.getPosition().first == newX && player.getPosition().second == newY) {
            std::cout << "Enemy attacked player!" << std::endl;
            player.takeDamage(enemy->getDamage());
            continue;
        }

        if (isPositionOccupiedByEnemy(newX, newY, enemies)) {
            continue;
        }

        if (grid.getCell(newX, newY).getType() == CellType::Trap) {
            enemy->takeDamage(30);
            std::cout << "Enemy triggered trap at (" << newX << "," << newY << ")!" << std::endl;
            
            grid.updateCell(currentX, currentY, CellType::Empty); 
            grid.updateCell(newX, newY, CellType::Empty);        
            
            continue;
        }

        updatePosition(currentX, currentY, newX, newY, CellType::Enemy);
        enemy->setPosition(newX, newY);
    }

    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
                  [](const std::unique_ptr<Enemy>& e){ return !e->isAlive(); }),
                  enemies.end());
}

bool MovementSystem::isValidMove(int x, int y) const {
    return grid.isValidCoordinate(x, y) && grid.isCellPassable(x, y);
}

bool MovementSystem::isPositionOccupiedByEnemy(int x, int y, const std::vector<std::unique_ptr<Enemy>>& enemies) const {
    for (const auto& enemy : enemies) {
        if (enemy && enemy->getPosition().first == x && enemy->getPosition().second == y) {
            return true;
        }
    }
    return false;
}

void MovementSystem::updatePosition(int oldX, int oldY, int newX, int newY, CellType type) {
    grid.updateCell(oldX, oldY, CellType::Empty);
    grid.updateCell(newX, newY, type);
}