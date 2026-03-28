#include "player.h"
#include "enemy.h"
#include "field.h"
#include "node.h"
#include "spellitems.h"
#include "hand.h"        
#include "console_renderer.h"
#include <iostream>
#include <cmath>
using namespace std;
Player::Player(std::string name, Field& field, int startX, int startY) 
    : Character(name, field, startX, startY), points(50), 
      combatType(CombatType::NEAR), combatPower(2), radius(1), hand(5){
    
    Spell* randomSpell = createRandomSpell();
    if (randomSpell) {
        hand.addSpell(randomSpell);
    }
}
Spell* Player::createRandomSpell() {
    int spellType = rand() % 5;
    switch(spellType) {
        case 0:
            return new Fireball();
        case 1:
            return new IceBolt();
        case 2:
            return new FireWave();
        case 3:
            return new IceStorm();
        case 4:
            return new PoisonTrap();
        default:
            return new Fireball();
    }
}
void Player::addPoints(int val) { points += val; }
void Player::removePoints(int val) { points -= val; }
int Player::getPoints() const { return points; }
int Player::getPower() const { return combatPower; }
int Player::getRadius() const { return radius; }
Player::CombatType Player::getCombatType() const {return combatType;}

void Player::turnCombat(CombatType type) {
    combatType = type;
    if (type == CombatType::NEAR) { 
        combatPower = 2; radius = 1; 
    } else { 
        combatPower = 1; radius = 3;
    }
}

bool Player::attack_force(Enemy& enemy, std::string& message) {
    bool res = false;
    int enemyX = enemy.getX();
    int enemyY = enemy.getY();
    
    int dx = abs(x - enemyX);
    int dy = abs(y - enemyY);
    int distance = (dx > dy) ? dx : dy;
    
    if (distance <= radius && enemy.isAlive()) {
        enemy.takeDamage(combatPower);
        message = "Вы атаковали " + enemy.getname() + "! Урон: " + to_string(enemy.getDamage()) + " Осталось жизней: " + to_string(enemy.getLives());
        addPoints(combatPower*2);
        res = true;
    } else {
        message = "Враг слишком далеко! Дистанция: " + to_string(distance);
        removePoints(combatPower);
        message += " Вы потеряли " + to_string(combatPower) + " очков за промах!";
    }
    
    return res;
}

bool Player::moveTo(int dx, int dy, std::string& message) {
    int newX = x + dx;
    int newY = y + dy;
    bool res = true;
    
    if (!field.validpos(newX, newY)) {
        message = "Выход за границы поля";
        res = false;
    }
    else if (!field.getNode(newX, newY).isPassable()) {
        message = "Клетка непроходима";
        res = false;
    }
    else if (field.getNode(newX, newY).hasEnemy()) {
        message = "На клетке находится враг! Нельзя пройти.";
        res = false;
    }
    
    if (res == true){
        if (field.getNode(newX, newY).hasTrap()) {
            int trapDamage = field.getNode(newX, newY).triggerTrap();
            this->takeDamage(trapDamage);
            message = "Вы наступили на ловушку! Урон: " + to_string(trapDamage);
        }
        field.getNode(x, y).removeEntity();
        x = newX;
        y = newY;
        field.getNode(x, y).setPlayer();
    }    
    return res;
}

bool Player::moveUp(std::string& message) { return moveTo(0, -1, message); }
bool Player::moveDown(std::string& message) { return moveTo(0, 1, message); }
bool Player::moveLeft(std::string& message) { return moveTo(-1, 0, message); }
bool Player::moveRight(std::string& message) { return moveTo(1, 0, message); }

// Методы для руки
Hand& Player::getHand() { return hand; }
const Hand& Player::getHand() const { return hand; }

bool Player::useSpell(int idx, int x, int y, Field& field, Enemy& enemy, string& message) {
    return hand.useSpell(idx, x, y, field, *this, enemy, message);
}