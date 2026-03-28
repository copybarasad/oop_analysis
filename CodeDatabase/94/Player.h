#ifndef PLAYER_H
#define PLAYER_H

#include "Character.h"
#include "Hand.h"
#include "Position.h"

enum class CombatMode {
    MELEE,
    RANGED
};

class Player : public Character {
private:
    int score;
    int meleeDamage;
    int rangedDamage;
    CombatMode currentCombatMode;
    Hand hand;
    Position position;

public:
    Player(int initialHealth, int meleeDmg, int rangedDmg);
    
    Player(const Player& other) = default;
    Player(Player&& other) = default;
    Player& operator=(const Player& other) = default;
    Player& operator=(Player&& other) = default;
    ~Player() override = default;
    
    int getScore() const;
    CombatMode getCombatMode() const;
    int getCurrentDamage() const;
    Hand& getHand() { return hand; }
    const Hand& getHand() const { return hand; }
    Position getPosition() const { return position; }
    void setPosition(const Position& pos) { position = pos; }
    
    // Добавленные методы для управления здоровьем
    void setHealth(int newHealth) { 
        Character::setHealth(newHealth);
    }
    
    void setMaxHealth(int newMaxHealth) { 
        Character::setMaxHealth(newMaxHealth);
    }
    
    void addScore(int points);
    void switchCombatMode();
    void heal(int amount);
    bool addSpell(std::unique_ptr<Spell> spell);
    
    Character* clone() const override;
    void performAction() override;
};

#endif