#include "Tower.h"
#include "Game.h"
#include "DamageSpell.h"
#include <iostream>

Tower::Tower(std::string id, Position pos, int hp, int attackRange, int damage, int cooldownTurns)
    : Entity(std::move(id), pos, hp)
    , attackRange_(attackRange)
    , damage_(damage)
    , cooldown_(0)
    , cooldownTurns_(cooldownTurns)
{}

void Tower::OnTurn(Game& game)
{
    if (cooldown_ > 0) {
        --cooldown_;
        return;
    }
    Position towerPos = GetPosition();
    Position playerPos = game.GetPlayerPosition();

    int dx = std::abs(towerPos.x - playerPos.x);
    int dy = std::abs(towerPos.y - playerPos.y);

    if (dx <= attackRange_ && dy <= attackRange_) {

        int weakenedDamage = damage_ / 2; 
        DamageSpell weakSpell("Tower Shot", weakenedDamage, attackRange_);

        std::cout << GetId() << " uses damage spell on player!\n";

        Player fakeCaster(towerPos, 0, 0, 0); 
        weakSpell.Apply(fakeCaster, game, playerPos);

        cooldown_ = cooldownTurns_;
    }
}


int Tower::GetRange() const { 
    return attackRange_; 
}

int Tower::GetDamage() const { 
    return damage_; 
}

bool Tower::IsAvailable() const { 
    return cooldown_ == 0; 
}


void Tower::SetCooldown(int v) {
    cooldown_ = v;
}

void Tower::TickCooldown() {
    if (cooldown_ > 0) --cooldown_;
}


//********
int Tower::GetCooldown() const {
    return cooldown_;
}

