#pragma once

#include "trap.hpp"
#include "spawn_spell.hpp"

class TrapSpell : public SpawnSpell {
public:
    TrapSpell();

    bool use(FieldHandler& field_handler, const Position& from, const Position& target) override;
};