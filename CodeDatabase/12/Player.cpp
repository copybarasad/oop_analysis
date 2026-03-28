#include "Player.h"
#include "Enemy.h"
#include "Field.h"
#include "Tower.h"
#include <iostream>
#include <algorithm>
#include <cmath>

Player::Player(int xStart, int yStart, int hp, int meleedm, int rangedm)
    : x(xStart), y(yStart), Health(hp), meleedamage(meleedm), rangedamage(rangedm),
      Alive(true), Ismelee(true) {}


void Player::move(char button, const Field& field) {
    if (!Alive) return;

    int newx = x, newy = y;

    switch (button) {
        case 'W': case 'w': newy--; break;
        case 'S': case 's': newy++; break; 
        case 'A': case 'a': newx--; break;
        case 'D': case 'd': newx++; break; 
        default:
            std::cout << "Неправильная команда!" << std::endl;
            return;
    }

    if (newx < 0 || newy < 0 || newx >= field.getWidth() || newy >= field.getHeight()) {
        std::cout << "Граница поля!" << std::endl;
        return;
    }

    if (!field.CanMove(newx, newy)) {
        std::cout << "Невозможно переместиться в эту клетку!" << std::endl;
        return;
    }

    x = newx;
    y = newy;
    std::cout << "Игрок переместился на клетку (" << x << "," << y << ")\n";

}

void Player::switch_attack() {
    if (!Alive) return;

    Ismelee = !Ismelee;
    std::cout << "Режим переключён на " << (Ismelee ? "БЛИЖНИЙ БОЙ" : "ДАЛЬНИЙ БОЙ") << std::endl;
    std::cout << "Урон: " << getCurrentDamage() << std::endl;
}


void Player::takeDamage(int dmg) {
    Health -= dmg;
    std::cout << "Игрок получил урон: " << dmg << ", HP: " << Health << std::endl;
    if (Health <= 0) {
        Alive = false;
        std::cout << "Игрок погиб" << std::endl;
    }
}

int Player::getDistanceTo(int targetX, int targetY) const {
    return std::abs(x - targetX) + std::abs(y - targetY);
}

bool Player::attack(std::vector<Enemy>& enemies) {

    bool hit = false;
    int currentdg = getCurrentDamage();

    for (auto& enemy : enemies) {
        int distance = getDistanceTo(enemy.getX(), enemy.getY());
        if (Ismelee) {
            if (distance <= MELEE_ATTACK_RANGE) {
                std::cout << "Игрок атакует врага в ближнем бою на (" << enemy.getX() << "," << enemy.getY() << ")\n";
                enemy.takeDamage(currentdg);
                hit = true;
            }
        } else {
            if (distance <= RANGED_ATTACK_RANGE && distance > 0) {
                std::cout << "Игрок атакует врага в дальнем бою на (" << enemy.getX() << "," << enemy.getY() << ")\n";
                enemy.takeDamage(currentdg);
                hit = true;
            }
        }
    }

    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [](const Enemy& enemy) { return !enemy.isAlive(); }),
        enemies.end()
    );

    if (!hit) {
        std::cout << "Нет врагов в зоне атаки\n";
        std::cout << (Ismelee ? "Ближний бой (радиус 1)" : "Дальний бой (радиус 3)") << std::endl;
    }
    return hit;
}

bool Player::attackTower(Tower& tower) {
    if (!tower.isAlive()) {
        std::cout << "Башня уже разрушена!\n";
        return false;
    }

    int towX = tower.getX();
    int towerY = tower.getY();
    int distance = getDistanceTo(towX, towerY);
    int currentdg = getCurrentDamage();

    int attackRange = Ismelee ? MELEE_ATTACK_RANGE : RANGED_ATTACK_RANGE;
    
    if (distance <= attackRange) {
        std::cout << "Игрок атакует башню на (" << towX << ", " << towerY << ")!\n";
        std::cout << "Расстояние: " << distance << ", радиус атаки: " << attackRange << "\n";
        tower.takeDamage(currentdg);
        return true;
    } else {
        std::cout << "Башня слишком далеко! Расстояние: " << distance << ", ваш радиус: " << attackRange << "\n";
        return false;
    }
}

void Player::heal(int amount){
    Health += amount;
}

int Player::getHealth() const { return Health; }
int Player::getCurrentDamage() const { return Ismelee ? meleedamage : rangedamage; }
int Player::getX() const { return x; }
int Player::getY() const { return y; }
int Player::getMeleeDamage() const { return meleedamage; }
int Player::getRangedDamage() const { return rangedamage; }
bool Player::isAlive() const { return Alive; }
bool Player::isMeleeMode() const { return Ismelee; }