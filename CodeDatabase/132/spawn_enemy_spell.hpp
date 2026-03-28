#pragma once

#include "enemy.hpp"
#include "spawn_spell.hpp"

class SpawnEnemySpell : public SpawnSpell {
public:
    SpawnEnemySpell();

    bool use(FieldHandler& field_handler, const Position& from, const Position& target) override;
};