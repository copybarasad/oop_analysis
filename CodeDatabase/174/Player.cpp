#include "Player.h"
#include "Enemy.h"
#include "EnemyBarrack.h"
#include "GameField.h"
#include "Constants.h"
#include "PlayerSpellHand.h"
#include <iostream>

using namespace std;

Player::Player(int x, int y) : Unit(DEFAULT_PLAYER_HEALTH, DEFAULT_PLAYER_MELEE_DAMAGE, x, y), 
                              scorePoints(0), 
                              fightingStyle(DEFAULT_FIGHTING_STYLE),
                              damageBonus(0),
                              attackRange(DEFAULT_MELEE_ATTACK_RANGE), 
                              skipNextMove(false),
                              hand(DEFAULT_HAND_SIZE) {
    if (fightingStyle == 'r') {
        damage = DEFAULT_PLAYER_RANGED_DAMAGE + damageBonus;
    } else {
        damage = DEFAULT_PLAYER_MELEE_DAMAGE + damageBonus;
    }
} 

int Player::getScorePoints() const { return scorePoints; }
char Player::getFightingStyle() const { return fightingStyle; }
int Player::getAttackRange() const { return attackRange; }
int Player::getDamageBonus() const { return damageBonus; }
int Player::getDamage() const { return damage; }
PlayerSpellHand& Player::getHand() { return hand; }
const PlayerSpellHand& Player::getHand() const { return hand; } 
bool Player::shouldSkipMove() const { return skipNextMove; }

void Player::resetSkipMove() { 
    skipNextMove = false; 
}

void Player::addScorePoints(int points) { 
    scorePoints += points; 
}

void Player::setFightingStyle(char newFightingStyle) { 
    fightingStyle = newFightingStyle; 
    if (fightingStyle == 'r') {
        damage = DEFAULT_PLAYER_RANGED_DAMAGE + damageBonus;
        attackRange = DEFAULT_RANGED_ATTACK_RANGE;
    } else {
        damage = DEFAULT_PLAYER_MELEE_DAMAGE + damageBonus;
        attackRange = DEFAULT_MELEE_ATTACK_RANGE;
    }
}

void Player::setSkipMove(bool skip) { 
    skipNextMove = skip; 
}

void Player::setDamageBonus(int bonus) {
    damageBonus = bonus;
    
    if (fightingStyle == 'r') {
        damage = DEFAULT_PLAYER_RANGED_DAMAGE + damageBonus;
    } else {
        damage = DEFAULT_PLAYER_MELEE_DAMAGE + damageBonus;
    }
}

bool Player::move(Direction direction, int maxWidth, int maxHeight, 
                  const vector<shared_ptr<Enemy>>& enemies,
                  const vector<shared_ptr<EnemyBuilding>>& buildings, 
                  const GameField& field) {
    if (skipNextMove) {
        skipNextMove = false;
        cout << "You are slowed and cannot move this turn!" << endl;
        return true;
    }
    
    Position newPosition = position;
    
    switch (direction) {
        case Direction::UP: 
            if (newPosition.getY() > 0) {
                newPosition.setY(newPosition.getY() - 1);
            }
            break;
        case Direction::DOWN: 
            if (newPosition.getY() < maxHeight - 1) {
                newPosition.setY(newPosition.getY() + 1);
            }
            break;
        case Direction::LEFT: 
            if (newPosition.getX() > 0) {
                newPosition.setX(newPosition.getX() - 1);
            }
            break;
        case Direction::RIGHT: 
            if (newPosition.getX() < maxWidth - 1) {
                newPosition.setX(newPosition.getX() + 1);
            }
            break;
        default: 
            return false;
    }
    
    if (newPosition == position) {
        return false;
    }
    
    if (!field.isValidPosition(newPosition) || field.isObstacleAt(newPosition.getX(), newPosition.getY())) {
        cout << "Cannot move there - obstacle!" << endl;
        return true;
    }
    
    for (const auto& enemy : enemies) {
        if (enemy && enemy->isAlive() && enemy->getPosition() == newPosition) {
            takeDamage(enemy->getDamage());
            cout << "You tried to move into enemy! Lost " << enemy->getDamage() << " HP." << endl;
            return true;
        }
    }
    
    for (const auto& building : buildings) {
        if (building && building->isAlive() && building->getPosition() == newPosition) {
            cout << "Cannot move there - enemy building is in the way!" << endl;
            return true;
        }
    }
    
    if (field.isSlowAt(newPosition.getX(), newPosition.getY())) {
        skipNextMove = true;
        cout << "You stepped on slow terrain! Next move will be skipped." << endl;
        position = newPosition;
        return true;
    }
    
    position = newPosition;
    return false;
}

CellType Player::getCellType() const { 
    return CellType::PLAYER; 
}