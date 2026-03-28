#ifndef PLAYER_H
#define PLAYER_H

#include "Position.h"
#include "SpellHand.h"

enum class AttackType {
    MELEE,
    RANGED
};

class Player {
private:
    int health;
    int maxHealth;
    int meleeDamage;
    int rangedDamage;
    int score;
    Position pos;
    bool alive;
    AttackType attackType;

    SpellHand spellHand;
    int mana;
    int maxMana;
    int invisibilityTurns;
    int enhancementPower;

public:
    Player(int health = 100, int meleeDmg = 15, int rangedDmg = 10, 
           int startMana = 50, int maxManaVal = 50);
    
    // Геттеры - ТОЛЬКО ОБЪЯВЛЕНИЯ
    int getHealth() const;
    int getDamage() const;
    int getMeleeDamage() const;
    int getRangedDamage() const;
    int getScore() const;
    const Position& getPos() const;
    bool isAlive() const;
    AttackType getAttackType() const;
    int getMaxHealth() const { return maxHealth; }  // можно оставить inline
    
    // Действия - ТОЛЬКО ОБЪЯВЛЕНИЯ
    void takeDamage(int damage);
    void addScore(int points);
    void move(int dx, int dy);
    void setPos(int x, int y);
    void switchAttackType();
    
    bool canAttack(const Position& targetPos, AttackType type) const;
    void checkAlive();

    // Заклинания - ТОЛЬКО ОБЪЯВЛЕНИЯ
    SpellHand& getSpellHand();
    const SpellHand& getSpellHand() const;
    
    int getMana() const;
    int getMaxMana() const;
    int getInvisibilityTurns() const;
    
    void addMana(int amount);
    void setInvisibilityTurns(int turns);
    void decrementInvisibility();
    
    bool canCastSpell() const;
    void castSpell(const Position& targetPos);

    // Система улучшений - можно оставить inline
    int getEnhancementPower() const { return enhancementPower; }
    void accumulateEnhancement() { enhancementPower++; }
    void resetEnhancement() { enhancementPower = 0; } 

    // Новые методы для улучшений - можно оставить inline
    void setMaxMana(int newMaxMana) { maxMana = newMaxMana; } 
    void setMeleeDamage(int damage) { meleeDamage = damage; } 
    void setRangedDamage(int damage) { rangedDamage = damage; }
    void setHealth(int newHealth) { health = newHealth; } 
    void setMaxHealth(int newMaxHealth) { maxHealth = newMaxHealth; } 
    
    void increaseMaxMana(float percentage) {
        maxMana += static_cast<int>(maxMana * percentage);
        mana = maxMana;
    }
    
    void increaseAttack(float percentage) {
        meleeDamage += static_cast<int>(meleeDamage * percentage);
        rangedDamage += static_cast<int>(rangedDamage * percentage);
    }
    
    void increaseSpellPower(float percentage) {
        // Реализация зависит от системы заклинаний
    }
};

#endif