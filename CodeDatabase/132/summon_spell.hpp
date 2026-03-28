#pragma once

#include "ally.hpp"
#include "spawn_spell.hpp"

class SummonSpell : public SpawnSpell {
public:
    SummonSpell();

    bool use(FieldHandler& field_handler, const Position& from, const Position& target) override;
};