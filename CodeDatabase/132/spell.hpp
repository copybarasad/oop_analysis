#pragma once

#include "ISpell.hpp"

class Spell: public ISpell {
private:
    char symbol;
public:
    Spell(char symbol);
    virtual char get_symbol() const override;
    virtual bool use(FieldHandler& field_handler, const Position& from, const Position& target) = 0;
    virtual int apply_enhancement(int current_lvl) = 0;
};