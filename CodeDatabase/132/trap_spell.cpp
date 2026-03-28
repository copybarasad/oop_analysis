#include "trap_spell.hpp"
#include "field_handler.hpp"

TrapSpell::TrapSpell(): SpawnSpell('_') {}

bool TrapSpell::use(FieldHandler& field_handler, const Position& from, const Position& target) {
    Trap* tr = new Trap(lvl);

    field_handler.place_obj(target, tr);

    return true;
}