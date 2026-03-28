#ifndef PLAYER_H
#define PLAYER_H

#include "position.h"
#include "playerhand.h"
#include <memory>

class GameField;

class Player {
private:
    int health;
    int damage;
    Position position;
    std::unique_ptr<PlayerHand> hand;
    int mana;
    int maxMana;
    int points;           // NEW: For buying spells
    int enemiesDestroyed; // NEW: Track enemies for spell rewards
    
public:
    Player(int health = 100, int damage = 15, const Position& startPosition = Position(0, 0));
    Player(const Player& other);
    Player(Player&& other) = default;
    Player& operator=(const Player& other);
    Player& operator=(Player&& other) = default;
    
    int getHealth() const;
    int getDamage() const;
    Position getPosition() const;
    void setHealth(int health);
    void setPosition(const Position& newPosition);
    void move(const Position& newPosition);
    bool isAlive() const;
    
    void setHand(std::unique_ptr<PlayerHand> newHand);
    bool castSpell(int spellIndex, GameField* field, const Position& targetPosition);
    void gainMana(int amount);
    bool canCastSpell(const Spell* spell) const;
    void takeDamage(int damage);
    
    // NEW: Point and enemy tracking methods
    void addPoints(int amount);
    bool spendPoints(int amount);
    int getPoints() const;
    void incrementEnemiesDestroyed();
    int getEnemiesDestroyed() const;
    
    PlayerHand* getHand() const;
    int getMana() const;
    int getMaxMana() const;

    void setMaxMana(int newMaxMana);
};

#endif