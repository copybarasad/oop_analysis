#include "Player.h"
#include "Field.h"
#include "Enemy.h"
#include <iostream>
#include <cstdlib>

Player::Player(int startX, int startY, int h, int melee, int ranged, int handSize)
    : Entity(startX, startY, h), meleeDamage(melee), rangedDamage(ranged),
      combatMode(CombatMode::Melee), score(0), hand(handSize) {}

Player::Player(const Player& other)
    : Entity(other), meleeDamage(other.meleeDamage), rangedDamage(other.rangedDamage),
      combatMode(other.combatMode), score(other.score), hand(other.hand) {}

Player& Player::operator=(const Player& other) {
    if (this == &other)
        return *this;
    
    Entity::operator=(other);
    meleeDamage = other.meleeDamage;
    rangedDamage = other.rangedDamage;
    combatMode = other.combatMode;
    score = other.score;
    hand = other.hand;
    
    return *this;
}

Player::Player(Player&& other) noexcept
    : Entity(std::move(other)), meleeDamage(other.meleeDamage), 
      rangedDamage(other.rangedDamage), combatMode(other.combatMode), 
      score(other.score), hand(std::move(other.hand)) {}

Player& Player::operator=(Player&& other) noexcept {
    if (this == &other)
        return *this;
    
    Entity::operator=(std::move(other));
    meleeDamage = other.meleeDamage;
    rangedDamage = other.rangedDamage;
    combatMode = other.combatMode;
    score = other.score;
    hand = std::move(other.hand);
    
    return *this;
}

bool Player::canMove(int dx, int dy, const Field& field) const {
    if (std::abs(dx) + std::abs(dy) != 1)
        return false;

    int newX = x + dx;
    int newY = y + dy;
    
    if (!field.isValidPosition(newX, newY))
        return false;
    
    if (field.isWall(newX, newY))
        return false;
    
    return true;
}

bool Player::move(int dx, int dy, Field& field) {
    if (!canMove(dx, dy, field))
        return false;
    
    int newX = x + dx;
    int newY = y + dy;
    
    if (field.hasEnemyAt(newX, newY)) {
        return attack(newX, newY, field);
    }
    
    field.clearCell(x, y);
    setPosition(newX, newY);
    field.placeEntity(this);
    
    return true;
}

bool Player::attack(int targetX, int targetY, Field& field) {
    auto enemy = field.getEnemyAt(targetX, targetY);
    if (!enemy) return false;
    
    enemy->takeDamage(getDamage());
    std::cout << "Вы атаковали врага! Урон: " << getDamage() << std::endl;
    
    if (!enemy->isAlive()) {
        std::cout << "Враг убит! +10 очков\n";
        addScore(10);
        field.removeEnemy(enemy);
    }
    
    return false;
}

void Player::switchCombatMode() {
    combatMode = (combatMode == CombatMode::Melee) ? CombatMode::Ranged : CombatMode::Melee;
    std::cout << "Игрок переключился в " << 
        (combatMode == CombatMode::Melee ? "Melee" : "Ranged") << " fight\n";
}

CombatMode Player::getCombatMode() const {
    return combatMode;
}

int Player::getDamage() const {
    return (combatMode == CombatMode::Melee) ? meleeDamage : rangedDamage;
}

int Player::getMeleeDamage() const {
    return meleeDamage;
}

int Player::getRangedDamage() const {
    return rangedDamage;
}

int Player::getScore() const {
    return score;
}

void Player::addScore(int points) {
    score += points;
}

void Player::setScore(int newScore) {
    score = newScore;
}

Hand& Player::getHand() {
    return hand;
}

const Hand& Player::getHand() const {
    return hand;
}

bool Player::castSpell(int spellIndex, int targetX, int targetY, Field& field) {
    Spell* spell = hand.getSpell(spellIndex);
    if (!spell) {
        std::cout << "Неверный индекс заклинания!\n";
        return false;
    }
    
    bool success = spell->cast(this, targetX, targetY, field);
    
    // Consume spell after casting (remove from hand)
    if (success) {
        hand.removeSpell(spellIndex);
    }
    
    return success;
}

char Player::getDisplayChar() const {
    return 'P';
}