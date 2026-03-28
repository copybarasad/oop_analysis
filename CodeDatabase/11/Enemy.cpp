#include "Enemy.h"
#include "Field.h"
#include "Player.h"
#include "EventLogger.h"
#include <cmath>
#include <algorithm>
#include <string>

Enemy::Enemy(int startX, int startY, int h, int dmg)
    : x(startX), y(startY), health(h), damage(dmg) {}

int Enemy::getX() const {return x;}
int Enemy::getY() const {return y;}
int Enemy::getHealth() const {return health;}
int Enemy::getDamage() const {return damage;}

bool Enemy::isAlive() const { return health > 0; }

void Enemy::takeDamage(int amount) {
    health -= amount;
    if (health < 0) health = 0;
}

void Enemy::processTurn(Player& player, Field& field, std::vector<Trap>& traps, EventLogger* logger) {
    int playerX = player.getX(), playerY = player.getY();
    int nextX = x, nextY = y;
    if (std::abs(playerX - x) > std::abs(playerY - y)) {    
        if (playerX > x) nextX++; else nextX--;             
    } else {
        if (playerY > y) nextY++; else nextY--;
    }

    if (field.isValidPosition(nextX, nextY)) {
        CellType targetCell = field.getCellType(nextX, nextY);
        
        if (targetCell == CellType::PLAYER) {
            player.takeDamage(damage);
            if (logger) {
                logger->log(LogMsg::ENEMY_HIT_PLAYER, damage);
            }
        } else if (targetCell == CellType::EMPTY || targetCell == CellType::SLOW || targetCell == CellType::TRAP){
            if (targetCell == CellType::TRAP) {
                auto it = std::find_if(traps.begin(), traps.end(), [&](const Trap& t){
                    return t.getX() == nextX && t.getY() == nextY;
                });
                if (it != traps.end()) {
                    this->takeDamage(it->getDamage());
                    if (logger) logger->log(LogMsg::ENEMY_STEPPED_TRAP);
                    traps.erase(it);
                }
            }
            if (isAlive()) {
                 move(nextX, nextY, field);
            }
        }
    }
}

void Enemy::move(int newX, int newY, Field& field) {
    if (field.getCellType(x, y) != CellType::SPAWNER) {
        field.setCellType(x, y, CellType::EMPTY);
    }
    x = newX; y = newY;
    field.setCellType(x, y, CellType::ENEMY);
    
}