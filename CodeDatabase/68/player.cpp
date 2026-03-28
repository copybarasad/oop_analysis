#include "player.h"
#include "gamefield.h"
#include "spell.h"
#include <iostream>

Player::Player(int health, int damage, const Position& startPosition) 
    : health(health), damage(damage), position(startPosition), 
      mana(50), maxMana(100), points(20), enemiesDestroyed(0) {  // Start with 20 points
        hand = std::unique_ptr<PlayerHand>(new PlayerHand());
}

Player::Player(const Player& other) 
    : health(other.health), damage(other.damage), position(other.position), 
      mana(other.mana), maxMana(other.maxMana), points(other.points), enemiesDestroyed(other.enemiesDestroyed) {
    hand = std::unique_ptr<PlayerHand>(new PlayerHand(other.hand->getMaxSize()));
}

Player& Player::operator=(const Player& other) {
    if (this != &other) {
        health = other.health;
        damage = other.damage;
        position = other.position;
        mana = other.mana;
        maxMana = other.maxMana;
        points = other.points;
        enemiesDestroyed = other.enemiesDestroyed;
        hand = std::unique_ptr<PlayerHand>(new PlayerHand(other.hand->getMaxSize()));
    }
    return *this;
}

int Player::getHealth() const {
    std::cout << "getHealth: returning: " << health << "\n"; 
    return health; 
}
int Player::getDamage() const { return damage; }
Position Player::getPosition() const { return position; }
void Player::setHealth(int newHealth) {
    std::cout << "DEbug setHealth: " << health << "->" << newHealth;
    this->health = newHealth;
    }
void Player::setPosition(const Position& newPosition) { position = newPosition; }

void Player::move(const Position& newPosition) {
    position = newPosition;
    std::cout << "Player moved to (" << position.x << ", " << position.y << ")\n";
}

bool Player::isAlive() const { return health > 0; }

void Player::setHand(std::unique_ptr<PlayerHand> newHand) {
    hand = std::move(newHand);
}

bool Player::castSpell(int spellIndex, GameField* field, const Position& targetPosition) {
    Spell* spell = hand->getSpell(spellIndex);
    if (!spell) {
        std::cout << "Invalid spell index!\n";
        return false;
    }
    
    if (!canCastSpell(spell)) {
        std::cout << "Not enough mana to cast " << spell->getName() << "!\n";
        return false;
    }
    
    int oldMAna = mana;
    mana -= spell->getManaCost();
    spell->cast(field, targetPosition);
    std::cout << "Spell cast! Mana remaining: " << mana << "/" << maxMana << "\n";
    return true;
}

void Player::gainMana(int amount) {
    mana += amount;
    if (mana > maxMana) mana = maxMana;
    std::cout << "Gained " << amount << " mana. Total: " << mana << "/" << maxMana << "\n";
}

bool Player::canCastSpell(const Spell* spell) const {
    return spell && mana >= spell->getManaCost();
}

void Player::takeDamage(int damage) {
    std::cout << "Before - health: " << health << ", damage: " << damage << '\n';
    health -= damage;
    if (health < 0) health = 0;
    std::cout << "after - health: " << health << ", damage: " << damage << '\n';
    std::cout << "Player takes " << damage << " damage! Health: " << health << "\n";
}

// NEW: Point system methods
void Player::addPoints(int amount) {
    points += amount;
    std::cout << "Gained " << amount << " points! Total: " << points << "\n";
}

bool Player::spendPoints(int amount) {
    if (points >= amount) {
        points -= amount;
        std::cout << "Spent " << amount << " points. Remaining: " << points << "\n";
        return true;
    }
    std::cout << "Not enough points! Need " << amount << ", have " << points << "\n";
    return false;
}

int Player::getPoints() const {
    return points;
}

void Player::incrementEnemiesDestroyed() {
    enemiesDestroyed++;
    std::cout << "Enemy destroyed! Total: " << enemiesDestroyed << "\n";
    
    // Lab requirement: "after destroying a certain number of enemies" - reward every 3 enemies
    if (enemiesDestroyed % 3 == 0) {
        std::cout << "🎉 Reward: Gained 10 points for destroying " << enemiesDestroyed << " enemies!\n";
        addPoints(10);
        
        // Also gain a free spell every 5 enemies
        if (enemiesDestroyed % 5 == 0 && hand) {
            std::cout << "🎁 Bonus: Free spell for destroying " << enemiesDestroyed << " enemies!\n";
            hand->gainRandomSpell();
        }
    }
}

int Player::getEnemiesDestroyed() const {
    return enemiesDestroyed;
}

PlayerHand* Player::getHand() const {
    return hand.get(); 
}
int Player::getMana() const { 
    return mana; 
}
int Player::getMaxMana() const { 
    return maxMana; 
}

void Player::setMaxMana(int newMaxMana) {
    maxMana = newMaxMana;
    // Ensure current mana doesn't exceed new max
    if (mana > maxMana) {
        mana = maxMana;
    }
    std::cout << "Max mana set to " << maxMana << "\n";
}