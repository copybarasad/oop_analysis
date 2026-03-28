#include "BuffSpell.h"
#include "Player.h"
#include "GameController.h"

bool BuffSpell::cast(Player& caster, GameController& game) {
    caster.addBuffCharge();
    game.logEvent(LogMsg::SPELL_BUFF);
    return true;
}

std::string BuffSpell::getName() const {
    return "BuffSpell";
}

std::string BuffSpell::getDescription() const {
    return "Enhances the next spell you cast.";
}