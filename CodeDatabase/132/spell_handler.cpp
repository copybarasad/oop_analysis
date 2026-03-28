#include "spell_handler.hpp"

SpellHandler::SpellHandler(FieldHandler& field_handler): field_handler(field_handler) {};

bool SpellHandler::cast_spell(const Position &from, const Position &target, std::shared_ptr<ISpell> spell) {
    return spell->use(field_handler, from, target);
}