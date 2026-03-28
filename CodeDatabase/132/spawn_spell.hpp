#pragma once

#include "spell.hpp"

class SpawnSpell : public Spell {
protected:
    int lvl;
public:
    SpawnSpell(char symbol);

    virtual bool use(FieldHandler& field_handler, const Position& from, const Position& target) = 0;
    virtual int apply_enhancement(int current_lvl) override;
};