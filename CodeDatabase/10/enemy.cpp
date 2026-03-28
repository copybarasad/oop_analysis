#include <iostream>
#include "enemy.h"
#include "player.h"
#include "field.h"
#include "cell.h"
#include "constants.h"
using namespace std;

Enemy::Enemy(string name, Field& field, int startX, int startY) 
    : Character(name, field, startX, startY, constants::ENEMY_INITIAL_LIVES), attackPower(constants::ENEMY_DAMAGE_POWER) {
    if (!field.placeEnemy(x, y)) {
        throw std::runtime_error("Невозможно разместить врага");
    }
}

bool Enemy::canAttack(int playerX, int playerY) {
    if (abs(playerX - x) <= 1 && abs(playerY - y) <= 1 && abs(playerX - x) != abs(playerY - y)) {
          return true;
    }
    return false;
}

bool Enemy::moveTowards(int playerX, int playerY) {
    
    vector<pair<int, int>> movementOptions; 
    if (playerX > x) movementOptions.push_back({1, 0});
    else if (playerX < x) movementOptions.push_back({-1, 0});
    if (playerY > y) movementOptions.push_back({0, 1});
    else if (playerY < y) movementOptions.push_back({0, -1});
    for (pair<int, int> direction : movementOptions) {
        int testX = x + direction.first;
        int testY = y + direction.second;
        bool is_on_trap = false;
        if (field.isValidPosition(testX, testY) && 
            field.getCell(testX, testY).isPassable()) {
            if (field.getCell(testX, testY).isDamageTrap()) {
                takeDamage(constants::TRAP_SPELL_POWER);
                field.getCell(testX, testY).setEmpty();
                is_on_trap = true;
            }
            field.getCell(x, y).removeEntity();
            x = testX;
            y = testY;
            field.getCell(x, y).setEnemy();
            cout << getname() << " переместился на (" << x << ", " << y << ")" << endl;
            if (is_on_trap) cout << getname() << " активировал ловушку и получил урон!" << endl;
            return true;
        }
    }
    
    vector<pair<int, int>> allDirections = {{1,0}, {-1,0}, {0,1}, {0,-1}};
    for (pair<int, int> direction : allDirections) {
        int testX = x + direction.first;
        int testY = y + direction.second;
        bool is_on_trap = false;
        if (field.isValidPosition(testX, testY) && 
            field.getCell(testX, testY).isPassable()) {
            if (field.getCell(testX, testY).isDamageTrap()) {
                takeDamage(constants::TRAP_SPELL_POWER);
                field.getCell(testX, testY).setEmpty();
                is_on_trap = true;
            }
            field.getCell(x, y).removeEntity();
            x = testX;
            y = testY;
            field.getCell(x, y).setEnemy();
            cout << getname() << " переместился на (" << x << ", " << y << ")" << endl;
            if (is_on_trap) cout << getname() << " активировал ловушку и получил урон!" << endl;
            return true;
        }
    }
    
    cout << getname() << " не может двигаться!" << endl;
    return false;
}

int Enemy::getAttackPower() { return attackPower; }

void Enemy::setPosition(int newX, int newY) { 
    field.getCell(x, y).removeEntity();
    x = newX; 
    y = newY;
    field.getCell(x, y).setEnemy();
}
void Enemy::setLives(int newLives) { lives = newLives; }
void Enemy::setDamage(int newDamage) { damage = newDamage; }
void Enemy::upgradeDamage(int increment) {
    attackPower += increment;
}
