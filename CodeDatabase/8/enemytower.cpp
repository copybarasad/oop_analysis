#include "enemytower.h"
#include <iostream>
#include <cmath>
using namespace std;

EnemyTower::EnemyTower(std::string name, int startX, int startY) 
    : name(name), x(startX), y(startY), 
      attackRange(5), canAttack(true), cooldown(3), currentCooldown(0), spell(5, 5, "Выстрел башни") {}

void EnemyTower::startCooldown(std::string& message) {
    canAttack = false;
    currentCooldown = cooldown;
    message = name + " перезаряжается...";
}

void EnemyTower::update(Player& player, std::string& message) {
    if (currentCooldown > 0) {
        currentCooldown--;
        if (currentCooldown == 0) {
            canAttack = true;
        }
    }
    if (canAttack) {
        tryAttackPlayer(player, message);
        message.clear();
    }
}

bool EnemyTower::tryAttackPlayer(Player& player, std::string& message) {
    if (!player.isAlive() || !canAttack) return false;
    int distance = max(abs(x - player.getX()), abs(y - player.getY()));
    if (distance <= attackRange) {
        string spellMessage;
        bool attackSuccess = spell.do_spell(player.getX(), player.getY(), player, spellMessage);
        if (attackSuccess) {
            startCooldown(message);
            if (!spellMessage.empty()) {
                message = spellMessage + " " + message;
            }
            return true;
        }
    }
    return false;
}

std::string EnemyTower::getName() const { return name; }
int EnemyTower::getX() const { return x; }
int EnemyTower::getY() const { return y; }
int EnemyTower::getAttackRange() const { return attackRange; }
bool EnemyTower::canAttackNow() const { return canAttack; }