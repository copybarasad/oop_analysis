#include "cell.h"
#include "player.h"
#include "enemy.h"

Cell::Cell() : type(CellType::EMPTY), playerRef(nullptr), enemyRef(nullptr) {}

CellType Cell::getType() const
{
    return type;
}

bool Cell::isEmpty() const
{
    return type == CellType::EMPTY;
}

bool Cell::isOccupied() const
{
    return !isEmpty();
}

void Cell::setPlayer(Player *player)
{
    clear();
    type = CellType::PLAYER;
    playerRef = player;
}

void Cell::setEnemy(Enemy *enemy)
{
    clear();
    type = CellType::ENEMY;
    enemyRef = enemy;
}

void Cell::clear()
{
    type = CellType::EMPTY;
    playerRef = nullptr;
    enemyRef = nullptr;
}

bool Cell::hasPlayer() const
{
    return type == CellType::PLAYER;
}

bool Cell::hasEnemy() const
{
    return type == CellType::ENEMY;
}