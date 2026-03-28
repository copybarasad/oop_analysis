#ifndef AOE_SPELL_H
#define AOE_SPELL_H

#include "Spell.h"
#include "Player.h"
#include "GameField.h"
#include "Enemy.h"
#include "EnemyBuilding.h"
#include "Game.h"
#include <memory>
#include <iostream>

class AoESpell : public Spell {
    int damage;
    int areaSize;

public:
    AoESpell(int dmg = DEFAULT_SPELL_AOE_DAMAGE, int area = DEFAULT_SPELL_AOE_AREA_SIZE);
    bool cast(GameField& field, const Position& center, Player& player, Game& game) override;
    std::string getName() const override;
    bool isBuffSpell() const override { return false; }
};

#endif