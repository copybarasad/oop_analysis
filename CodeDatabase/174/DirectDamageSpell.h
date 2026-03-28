#ifndef DIRECT_DAMAGE_SPELL_H
#define DIRECT_DAMAGE_SPELL_H

#include "Spell.h"
#include "Player.h"
#include "GameField.h"
#include "CombatManager.h"
#include "Enemy.h"
#include "EnemyBuilding.h"
#include "Game.h"
#include <memory>
#include <iostream>

class DirectDamageSpell : public Spell {
    int damage;
    int range;

public:
    DirectDamageSpell(int dmg = DEFAULT_SPELL_DIRECT_DAMAGE, int range = DEFAULT_SPELL_DIRECT_RANGE);
    bool cast(GameField& field, const Position& targetPos, Player& player, Game& game) override;
    std::string getName() const override;
    bool isBuffSpell() const override { return false; }
};

#endif