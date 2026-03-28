#pragma once
#include "Entity.h"
#include <string>

class Game;

class Tower : public Entity 
{
public:
    Tower(std::string id, Position pos, int hp, int attackRange, int damage, int cooldownTurns);
    void OnTurn(Game& game) override;
    int GetRange() const;
    int GetDamage() const;
    bool IsAvailable() const;
    void SetCooldown(int v);
    void TickCooldown();

    int GetCooldown() const;

private:
    int attackRange_;
    int damage_;
    int cooldown_;
    int cooldownTurns_;
};