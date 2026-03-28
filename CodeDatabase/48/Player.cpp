#include "lib/Player.h"
#include "lib/Enemy.h"
#include "lib/GameField.h"
#include "lib/DirectDamageSpell.h"
#include "lib/AreaDamageSpell.h"
#include "lib/TrapSpell.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>

Player::Player(GameField* gameField, const std::string& playerName, int startX, int startY) 
    : field(gameField),
      name(playerName),
      health(100),
      maxHealth(100),
      damage(25),
      score(0),
      level(1),
      x(startX),
      y(startY),
      isAlive(true),
      mana(50),
      maxMana(50),
      spellHand(5) {
    
    // Add one random starting spell
    addRandomSpell();
    
    if (field && field->isValidPosition(x, y)) {
        field->setPlayerPosition(x, y);
    }
}

// Геттеры
std::string Player::getName() const {
    return name;
}

int Player::getHealth() const {
    return health;
}

int Player::getMaxHealth() const {
    return maxHealth;
}

int Player::getDamage() const {
    return damage;
}

int Player::getScore() const {
    return score;
}

int Player::getLevel() const {
    return level;
}

int Player::getX() const {
    return x;
}

int Player::getY() const {
    return y;
}

bool Player::isPlayerAlive() const {
    return isAlive;
}

int Player::getMana() const {
    return mana;
}

int Player::getMaxMana() const {
    return maxMana;
}

SpellHand& Player::getSpellHand() {
    return spellHand;
}

const SpellHand& Player::getSpellHand() const {
    return spellHand;
}

bool Player::isValidMove(int newX, int newY) const {
    return field && field->isValidPosition(newX, newY) && 
           field->isCellPassable(newX, newY) && 
           !field->getCell(newX, newY).getHasEnemy();
}

bool Player::move(int dx, int dy) {
    if (!isAlive || !field) {
        std::cout << name << " is dead and cannot move!\n";
        return false;
    }
    
    int newX = x + dx;
    int newY = y + dy;
    
    if (isValidMove(newX, newY)) {
        field->clearCell(x, y);
        x = newX;
        y = newY;
        field->setPlayerPosition(x, y);
        std::cout << name << " moved to position (" << x << ", " << y << ")\n";
        
        // Restore some mana when moving
        restoreMana(2);
        return true;
    } else {
        std::cout << "Cannot move to position (" << newX << ", " << newY << ") - cell is blocked!\n";
        return false;
    }
}

void Player::setPosition(int newX, int newY) {
    if (isAlive && field && field->isValidPosition(newX, newY) && field->isCellPassable(newX, newY)) {
        if (field->isValidPosition(x, y)) {
            field->clearCell(x, y);
        }
        
        x = newX;
        y = newY;
        field->setPlayerPosition(x, y);
    }
}

void Player::takeDamage(int damageAmount) {
    if (!isAlive || damageAmount <= 0) return;
    
    health -= damageAmount;
    std::cout << name << " received " << damageAmount << " damage! Health: " << health << "/" << maxHealth << "\n";
    
    if (health <= 0) {
        health = 0;
        isAlive = false;
        if (field) {
            field->clearCell(x, y);
        }
        std::cout << "💀 " << name << " died! Game over.\n";
    }
}

void Player::setDamage(int newDamage) {
    if (newDamage > 0) {
        damage = newDamage;
    }
}

void Player::heal(int healAmount) {
    if (!isAlive) {
        std::cout << name << " is dead and cannot be healed!\n";
        return;
    }
    
    health += healAmount;
    if (health > maxHealth) {
        health = maxHealth;
    }
    std::cout << name << " healed " << healAmount << " health. Health: " << health << "/" << maxHealth << "\n";
}

void Player::setHealth(int newHealth) {
    if (newHealth >= 0 && newHealth <= maxHealth) {
        health = newHealth;
    }
}

void Player::setMaxHealth(int newMaxHealth) {
    if (newMaxHealth > 0) {
        maxHealth = newMaxHealth;
        if (health > maxHealth) {
            health = maxHealth;
        }
    }
}

void Player::setMana(int newMana) {
    if (newMana >= 0 && newMana <= maxMana) {
        mana = newMana;
    }
}

void Player::setMaxMana(int newMaxMana) {
    if (newMaxMana > 0) {
        maxMana = newMaxMana;
        if (mana > maxMana) {
            mana = maxMana;
        }
    }
}

void Player::setScore(int newScore) {
    if (newScore >= 0) {
        score = newScore;
    }
}

void Player::setLevel(int newLevel) {
    if (newLevel > 0) {
        level = newLevel;
    }
}

void Player::increaseScore(int points) {
    if (isAlive && points > 0) {
        score += points;
        std::cout << name << " gained " << points << " points! Total score: " << score << "\n";
        
        // Every 50 points, player gets a new spell
        if (score % 50 == 0) {
            addRandomSpell();
        }
        
        // Every 100 points, player levels up
        if (score % 100 == 0) {
            levelUp();
        }
    }
}

void Player::levelUp() {
    if (!isAlive) return;
    
    level++;
    maxHealth += 20;
    health = maxHealth;
    damage += 5;
    maxMana += 10;
    mana = maxMana;
    
    std::cout << "🎉 " << name << " leveled up to " << level << "!\n";
    std::cout << "Max health: " << maxHealth << ", Damage: " << damage << ", Max mana: " << maxMana << "\n";
}

void Player::displayStatus() const {
    std::cout << "\n=== Player Status ===\n";
    std::cout << "Name: " << name << "\n";
    std::cout << "Level: " << level << "\n";
    std::cout << "Health: " << health << "/" << maxHealth << "\n";
    std::cout << "Mana: " << mana << "/" << maxMana << "\n";
    std::cout << "Damage: " << damage << "\n";
    std::cout << "Score: " << score << "\n";
    std::cout << "Position: (" << x << ", " << y << ")\n";
    std::cout << "Spells: " << spellHand.getSpellCount() << "/" << spellHand.getMaxSize() << "\n";
    std::cout << "Status: " << (isAlive ? "Alive" : "Dead") << "\n";
    std::cout << "=====================\n";
}

void Player::attackEnemy(Enemy& enemy) {
    std::cout << "=== DEBUG ATTACK START ===\n";
    std::cout << "Player attacking enemy at position (" << enemy.getX() << "," << enemy.getY() << ")\n";
    
    if (!isAlive || !enemy.isAlive()) {
        std::cout << "DEBUG: Cannot attack - player alive: " << isAlive << ", enemy alive: " << enemy.isAlive() << "\n";
        return;
    }
    
    int distanceX = std::abs(enemy.getX() - x);
    int distanceY = std::abs(enemy.getY() - y);
    
    std::cout << "DEBUG: Player pos (" << x << "," << y << "), Enemy pos (" << enemy.getX() << "," << enemy.getY() << ")\n";
    std::cout << "DEBUG: Distance X=" << distanceX << ", Y=" << distanceY << "\n";
    
    if (distanceX <= 1 && distanceY <= 1) {
        std::cout << "DEBUG: Enemy in range! Dealing " << damage << " damage\n";
        enemy.takeDamage(damage);
        std::cout << "DEBUG: After attack - enemy alive: " << enemy.isAlive() << "\n";
    } else {
        std::cout << "DEBUG: Enemy too far!\n";
    }
    std::cout << "=== DEBUG ATTACK END ===\n";
}

bool Player::castSpell(int spellIndex, int targetX, int targetY) {
    if (!isAlive) {
        std::cout << name << " is dead and cannot cast spells!\n";
        return false;
    }
    
    SpellCard* spell = spellHand.getSpell(spellIndex);
    if (spell == nullptr) {
        std::cout << "No spell in that slot!\n";
        return false;
    }
    
    if (mana < spell->getManaCost()) {
        std::cout << "Not enough mana to cast " << spell->getName() << "! Need " 
                  << spell->getManaCost() << " mana, but have " << mana << ".\n";
        return false;
    }
    
    // Cast the spell and check if it was successful
    bool success = spell->cast(*this, targetX, targetY, field);
    if (success) {
        mana -= spell->getManaCost();
        std::cout << "Mana remaining: " << mana << "/" << maxMana << "\n";
    }
    return success;
}

void Player::addRandomSpell() {
    if (spellHand.getSpellCount() >= spellHand.getMaxSize()) {
        std::cout << "Spell hand is full! Cannot learn new spell.\n";
        return;
    }
    
    // Initialize random seed if not already done
    static bool seeded = false;
    if (!seeded) {
        srand(time(0));
        seeded = true;
    }
    
    // Simple random spell selection
    int spellType = rand() % 3;
    SpellCard* newSpell = nullptr;
    
    switch (spellType) {
        case 0:
            newSpell = new DirectDamageSpell("Fire Bolt", 15 + level * 2, 10, 3 + level);
            break;
        case 1:
            newSpell = new AreaDamageSpell("Fireball", 10 + level, 15, 4 + level, 2);
            break;
        case 2:
            newSpell = new TrapSpell("Bear Trap", 20 + level * 3, 8, 2, 5);
            break;
    }
    
    if (newSpell && spellHand.addSpell(newSpell)) {
        std::cout << "📖 Learned new spell: " << newSpell->getName() << "!\n";
        std::cout << "Description: " << newSpell->getDescription() << "\n";
    }
    
    delete newSpell; // spellHand makes a clone
}

void Player::restoreMana(int amount) {
    if (amount > 0) {
        mana += amount;
        if (mana > maxMana) {
            mana = maxMana;
        }
        std::cout << name << " restored " << amount << " mana. Current mana: " << mana << "/" << maxMana << "\n";
    }
}

double Player::calculateDistanceTo(int targetX, int targetY) const {
    return sqrt(pow(targetX - x, 2) + pow(targetY - y, 2));
}