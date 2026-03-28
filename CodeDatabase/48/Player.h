#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "SpellHand.h"

// Forward declarations
class Enemy;
class GameField;
class SpellCard;

class Player {
private:
    GameField* field; // ссылка на игровое поле - moved first
    std::string name;
    int health;
    int maxHealth;
    int damage;
    int score;
    int level;
    int x;
    int y;
    bool isAlive;
    int mana;
    int maxMana;
    SpellHand spellHand;

public:
    // Constructor
    Player(GameField* gameField, const std::string& playerName, int startX = 0, int startY = 0);
    
    // Getters
    std::string getName() const;
    int getHealth() const;
    int getMaxHealth() const;
    int getDamage() const;
    int getScore() const;
    int getLevel() const;
    int getX() const;
    int getY() const;
    bool isPlayerAlive() const;
    int getMana() const;
    int getMaxMana() const;
    SpellHand& getSpellHand();
    const SpellHand& getSpellHand() const;
    
    // Setters
    void setPosition(int newX, int newY);
    void setDamage(int newDamage);
    void setHealth(int newHealth);
    void setMaxHealth(int newMaxHealth);
    void setMana(int newMana);
    void setMaxMana(int newMaxMana);
    void setScore(int newScore);
    void setLevel(int newLevel);
    
    // Action methods
    bool move(int dx, int dy);
    void takeDamage(int damageAmount);
    void heal(int healAmount);
    void increaseScore(int points);
    void levelUp();
    void displayStatus() const;
    void attackEnemy(Enemy& enemy);
    bool castSpell(int spellIndex, int targetX, int targetY);
    void addRandomSpell();
    void restoreMana(int amount);
    
    // Helper methods
    double calculateDistanceTo(int targetX, int targetY) const;

private:
    // Move validation
    bool isValidMove(int newX, int newY) const;
};

#endif