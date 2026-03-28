#include "Enemy.h"
#include "GameField.h"
#include "Player.h"
#include <iostream>
#include <cmath>

Enemy::Enemy(int health, int damage, int x, int y)
    : BaseEnemy(health, damage, x, y) {}

void Enemy::makeMove(GameField& field, Player& player) {
    if (!isAlive()) return;

    int playerX = player.getPositionX();
    int playerY = player.getPositionY();

    if (canAttackPlayer(player)) {
        player.takeDamage(getDamage());
        std::cout << "Enemy attacks player for " << getDamage() << " damage!" << std::endl;
        return;
    }

    int deltaX = 0;
    int deltaY = 0;

    if (playerX > getPositionX()) deltaX = 1;
    else if (playerX < getPositionX()) deltaX = -1;

    if (playerY > getPositionY()) deltaY = 1;
    else if (playerY < getPositionY()) deltaY = -1;

    int diffX = std::abs(playerX - getPositionX());
    int diffY = std::abs(playerY - getPositionY());

    int newX = getPositionX();
    int newY = getPositionY();

    if (diffX > diffY) {
        newX += deltaX;
    }
    else {
        newY += deltaY;
    }

    if (field.isValidPosition(newX, newY) && field.isCellPassable(newX, newY)) {
        if (field.hasTrap(newX, newY)) {
            field.triggerTrap(newX, newY, *this);  // Используем метод GameField
            return;
        }

        if (newX == player.getPositionX() && newY == player.getPositionY()) {
            player.takeDamage(getDamage());
            std::cout << "Enemy attacks player for " << getDamage() << " damage!" << std::endl;
        }
        else {
            setPosition(newX, newY);
        }
    }
}