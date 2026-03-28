#pragma once
#include "Character.h"
#include "PlayerHand.h"
#include <iostream>

enum class CombatMode {
    Melee,
    Ranged
};

class Player : public Character {
private:
    int score;
    CombatMode combatMode;
    int meleeDamage;
    int rangedDamage;
    bool isSlowed; 

public:
    // Конструктор
    Player(int health, int damage, int startX, int startY);

    // Геттеры
    int getScore() const;
    CombatMode getCombatMode() const;
    int getCurrentDamage() const;

    // Действия 
    void switchCombatMode(GameState& state);
    void addScore(int points);
    void setScore(int newScore);
    void setCombatMode(int newMode);

    // Передвижение
    void move(int deltaX, int deltaY, const GameField& field) override;
};