#include "lib/Cell.h"

Cell::Cell(bool passable) {
    isPassable = passable;
    hasPlayer = false;
    hasEnemy = false;
    hasTower = false;
    updateSymbol();
}

// Геттеры
bool Cell::getIsPassable() const {
    return isPassable;
}

bool Cell::getHasPlayer() const {
    return hasPlayer;
}

bool Cell::getHasEnemy() const {
    return hasEnemy;
}

bool Cell::getHasTower() const {
    return hasTower;
}

char Cell::getSymbol() const {
    return symbol;
}

// Сеттеры
void Cell::setIsPassable(bool passable) {
    isPassable = passable;
    updateSymbol();
}

void Cell::setHasPlayer(bool player) {
    hasPlayer = player;
    updateSymbol();
}

void Cell::setHasEnemy(bool enemy) {
    hasEnemy = enemy;
    updateSymbol();
}

void Cell::setHasTower(bool tower) {
    hasTower = tower;
    updateSymbol();
}

// Обновление символа клетки
void Cell::updateSymbol() {
    if (hasPlayer) {
        symbol = 'P'; // Player
    } else if (hasEnemy) {
        symbol = 'E'; // Enemy
    } else if (hasTower) {
        symbol = 'T'; // Tower
    } else if (!isPassable) {
        symbol = '#'; // Wall/Obstacle
    } else {
        symbol = '.'; // Empty cell
    }
}

// Проверка пуста ли клетка
bool Cell::isEmpty() const {
    return isPassable && !hasPlayer && !hasEnemy && !hasTower;
}