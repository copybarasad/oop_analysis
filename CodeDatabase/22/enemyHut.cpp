#include "characters/enemyHut.h"
#include "characters/enemy.h" 

EnemyHut::EnemyHut(int health, Position position, int counter)
    : Character(health, 0, position), moveCounter(counter) {
        this->move = moveCounter;
    }

std::unique_ptr<Enemy> EnemyHut::generateEnemy()
{
    Position new_pos = this->getPosition();
    new_pos.y++;
    return std::make_unique<Enemy>(100, 10, new_pos);
}

bool EnemyHut::update()
{
    move--;
    if (move <= 0)
    {
        move = moveCounter;
        return true;
    }
    
    return false;
}


int EnemyHut::getMove(){return move;}
