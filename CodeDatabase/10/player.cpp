#include "player.h"
#include "enemy.h"
#include "field.h"
#include "cell.h"
#include "constants.h"
#include <iostream>
#include <cmath>
using namespace std;

Player::Player(std::string name, Field& field, int startX, int startY, int handSize = 5) 
    : Character(name, field, startX, startY, constants::PLAYER_INITIAL_LIVES), score(0), 
      combatStyle(CombatStyle::MELEE), attackStrength(constants::PLAYER_NEAR_COMBAT_POWER), 
      attackRange(constants::PLAYER_NEAR_RADIUS), slowed(false), slowTimer(0),  spellHand(handSize) {
    if (!field.placePlayer(x, y)) {
        throw std::runtime_error("Невозможно разместить игрока");
    }
}

void Player::addScore(int value) { score += value; }
int Player::getScore() const { return score; }
int Player::getPower() const { return attackStrength; }
int Player::getRadius() const { return attackRange; }
Player::CombatStyle Player::getCombatType() const {return combatStyle;}

void Player::switchCombatStyle(CombatStyle style) {
    combatStyle = style;
    if (style == CombatStyle::MELEE) { 
        attackStrength = constants::PLAYER_NEAR_COMBAT_POWER; 
        attackRange = constants::PLAYER_NEAR_RADIUS; 
        cout << "Переключен на ближний бой" << endl;
    } else { 
        attackStrength = constants::PLAYER_FAR_COMBAT_POWER; 
        attackRange = constants::PLAYER_FAR_RADIUS;
        cout << "Переключен на дальний бой" << endl;
    }
}

bool Player::canAttack(int targetX, int targetY, std::string enemyName) {
    int dx = abs(x - targetX);
    int dy = abs(y - targetY);
    if (dx == dy && dx == 1) {
        cout << "Цель вне досягаемости! Атаковать по диагонали нельзя!" << endl;
        return false;
    }
    
    int distance = (dx > dy) ? dx : dy;
    
    if (distance <= attackRange) {
        cout << "Атакован " << enemyName << "! Нанесено урона: " << attackStrength << endl;
        return true;
    } else {
        cout << "Цель вне досягаемости! Расстояние: " << distance << endl;
    }
    return false;
}


bool Player::isSlowedDown() const {
    return slowed;
}

void Player::applySlowEffect() {
    slowed = true;
    slowTimer = constants::SLOW_TRAP_DURATION;
    cout << getname() << " активировал ловушку замедления! Пропуск хода.\n";
}

void Player::updateSlowEffect() {
    if (slowed) {
        slowTimer--;
        if (slowTimer <= 0) {
            slowed = false;
            cout << getname() << " восстановил скорость.\n";
        }
    }
}

bool Player::move(int deltaX, int deltaY) {
    if (slowed) {
        cout << "Действие замедления! Движение невозможно.\n";
        return true;
    }
    
    int newX = x + deltaX;
    int newY = y + deltaY;
    bool success = true;
    
    if (!field.isValidPosition(newX, newY)) {
        cout << "Недопустимые координаты" << endl;
        success = false;
    }
    if (!field.isCellFree(newX, newY)) {
        cout << "Клетка занята" << endl;
        success = false;
    }
    if (success){
        field.getCell(x, y).removeEntity();
        x = newX;
        y = newY;
        if (field.getCell(x, y).isTrap()) {
            applySlowEffect();
        }
        field.getCell(x, y).setPlayer();
    }
    return success;
}

bool Player::moveUp() { return move(0, -1); }
bool Player::moveDown() { return move(0, 1); }
bool Player::moveLeft() { return move(-1, 0); }
bool Player::moveRight() { return move(1, 0); }

int Player::castSpell(int spellIndex, int targetX, int targetY, int enemyX, int enemyY) {
    return spellHand.useSpell(spellIndex, targetX, targetY, enemyX, enemyY, field, *this);
}

bool Player::buyNewSpell() {
    return spellHand.buySpell(score);
}

void Player::showSpells() const {
    spellHand.showHand();
}

bool Player::removeSpell(int index) {
    return spellHand.removeSpell(index);
}

Hand& Player::getSpellHand() { return spellHand; }

void Player::setPosition(int newX, int newY) { 
    field.getCell(x, y).removeEntity();
    x = newX; 
    y = newY;
    field.getCell(x, y).setPlayer();
}
void Player::setLives(int newLives) { lives = newLives; }
void Player::setScore(int newScore) { score = newScore; }
void Player::setCombatType(CombatStyle style) { 
    combatStyle = style; 
    if (style == CombatStyle::MELEE) { 
        attackStrength = constants::PLAYER_NEAR_COMBAT_POWER; 
        attackRange = constants::PLAYER_NEAR_RADIUS; 
    } else { 
        attackStrength = constants::PLAYER_FAR_COMBAT_POWER; 
        attackRange = constants::PLAYER_FAR_RADIUS;
    }
}
void Player::upgradeDamage(int increment) {
    attackStrength += increment;
}
