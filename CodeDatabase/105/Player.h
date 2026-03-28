#pragma once

#include "enums.h"
#include "SpellHand.h"

class Spell;      
class GameField;  

class Player {
private:
    int health;
    int maxHealth;
    int meleeDamage;
    int rangedDamage;
    int score;
    CombatType combatType;
    bool canMove;
    SpellHand spellHand;
    int mana;
    int maxMana;

public:
    Player(int startHealth = 100, int meleeDmg = 20, int rangedDmg = 15);
    
    // Геттеры
    int getHealth() const;
    int getMaxHealth() const;
    int getDamage() const;
    int getScore() const;
    int getMeleeDamage() const;
    int getRangedDamage() const;
    CombatType getCombatType() const;
    bool canMoveNextTurn() const;
    
    // Сеттеры
    void takeDamage(int damage);
    void addScore(int points);
    void heal(int amount);
    void switchCombatType();
    void resetMovement();
    void setMovement(bool movement);
    bool isAlive() const;

     // Новые методы для заклинаний
    SpellHand& getSpellHand();
    const SpellHand& getSpellHand() const;
    int getMana() const;
    int getMaxMana() const;
    void setMana(int newMana);
    void restoreMana(int amount);
    bool canCastSpell(const Spell& spell) const;
    bool buySpell(std::unique_ptr<Spell> spell, int cost);
    
    // Дополнительные методы для системы заклинаний
    bool castSpell(int spellIndex, GameField& field, int targetX, int targetY);
    bool hasSpells() const;
    void displaySpellInfo() const;

    // Для сериализации
    void setHealth(int hp);
    void setMaxHealth(int maxHp);
    void setMaxMana(int maxMp);
    void setScore(int newScore);
    void setCombatType(CombatType type);
    void clearSpells();
    void setMeleeDamage(int damage);
    void setRangedDamage(int damage);
};