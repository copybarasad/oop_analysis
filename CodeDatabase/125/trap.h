#pragma once
#include "position.h"

class Player;
class EnemyManager;
class GameField;

class Trap {
public:
    explicit Trap(int damage = 0);
    int GetDamage() const;
    bool IsActive() const;
    void Trigger();
    
    static void CastTrapSpell(Player& player, EnemyManager& enemies, GameField& field);

private:
    int damage_;
    bool active_;
};