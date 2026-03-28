#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"
#include "FightMode.h"
#include "SpellHand.h"

class Player : public GameObject {
private:
    int score;
    int baseDamage;
    FightMode currentMode;
    int meleeDamage;
    int rangedDamage;
    int mana;
    int maxMana;
    int rangedAttackRadius;
    bool skipNextTurn;

public:
    Player(int health = 100, int meleeDmg = 15, int rangedDmg = 10, int mana = 50);
    
    int getScore() const;
    void addScore(int points);
    
    FightMode getFightMode() const;
    void switchFightMode();
    
    int getDamage() const;
    int getMeleeDamage() const;
    int getRangedDamage() const;
    int getRangedAttackRadius() const;
    
    // Новые методы для прокачки
    void setMeleeDamage(int damage);
    void setRangedDamage(int damage);
    void setMaxHealth(int newMaxHealth);
    
    int getMana() const;
    int getMaxMana() const;
    void setMana(int newMana);
    void setMaxMana(int newMaxMana);  // ДОБАВЛЯЕМ ЭТОТ МЕТОД
    bool useMana(int amount);
    void restoreMana(int amount);
    
    std::string getType() const override;
    
    bool shouldSkipTurn() const;
    void setSkipTurn(bool skip);
    void resetTurnSkip();
};

#endif
