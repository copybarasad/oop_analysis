#include "Enemy.h"
#include "Player.h"
#include "Field.h"
#include <iostream>
#include <cstdlib>

Enemy::Enemy(int Xstart, int Ystart, int hp, int dm)
    : x(Xstart), y(Ystart), Health(hp), Damage(dm), Alive(true) {}

void Enemy::move(Player& player, Field& field, std::vector<Enemy>& allEnemies) {
    if (!Alive) return;

    int playerX = player.getX();
    int playerY = player.getY();
    
    field.checkTrap(x, y, allEnemies);
    if (!Alive) return; 
    
    int distance = abs(playerX - x) + abs(playerY - y);
    
    if (distance == 1) {
        std::cout << "Враг атакует игрока с позиции (" << x << ", " << y << ")!" << std::endl;
        player.takeDamage(Damage);
        return;
    }
    
    if (distance > 1) {
        int newx = x;
        int newy = y;

        if (abs(playerX - x) > abs(playerY - y)) {
            if (playerX < x) newx--;
            else if (playerX > x) newx++;
        } else {
            if (playerY < y) newy--;
            else if (playerY > y) newy++;
        }

        if (newx >= 0 && newy >= 0 && newx < field.getWidth() && newy < field.getHeight() && 
            field.CanMove(newx, newy)) {
            
            if (newx == playerX && newy == playerY) {
                std::cout << "Враг атакует игрока при движении!" << std::endl;
                player.takeDamage(Damage);
            } else {
                field.checkTrap(newx, newy, allEnemies);
                
                if (Alive) {
                    x = newx;
                    y = newy;
                }
            }
        }
    }
}

void Enemy::takeDamage(int dmg) {
    if (!Alive) return;

    Health -= dmg;
    std::cout << "Враг получил урон: " << dmg << ", HP: " << Health << std::endl;

    if (Health <= 0) {
        Alive = false;
        std::cout << "Враг убит!" << std::endl;
    }
}

int Enemy::getX() const { return x; }
int Enemy::getY() const { return y; }
int Enemy::getHealth() const { return Health; }
int Enemy::getDamage() const { return Damage; }
bool Enemy::isAlive() const { return Alive; }