#pragma once
#include "field.hpp"
#include <memory>
#include "ISpell.hpp"

class SpellHandler {
private:
    FieldHandler& field_handler;
public:
    SpellHandler(FieldHandler& field_handler);
    bool cast_spell(const Position &from, const Position &target, std::shared_ptr<ISpell> spell);
};