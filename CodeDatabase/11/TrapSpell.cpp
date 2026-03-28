#include "TrapSpell.h"
#include "GameController.h"
#include "Logger.h"

bool TrapSpell::cast(Player& caster, GameController& game) {
    int targetX = game.getInputInt("Enter X coordinate to place trap: ");
    int targetY = game.getInputInt("Enter Y coordinate to place trap: ");

    if (!game.getField().isValidPosition(targetX, targetY) || game.getField().getCellType(targetX, targetY) != CellType::EMPTY) {
        game.logEvent(LogMsg::SPELL_TRAP_FAIL);
        return false;
    }
    
    int damage = 40;
    
    int buffCharges = caster.getBuffCharges();
    if (buffCharges > 0) {
        damage += 20 * buffCharges;
        game.logEvent(LogMsg::SPELL_BUFF);
        caster.consumeBuffCharges();
    }
    
    game.placeTrap(targetX, targetY, damage);
    game.logEvent(LogMsg::SPELL_TRAP_PLACED);
    
    return true;
}

std::string TrapSpell::getName() const {
    return "Trap Spell";
}
std::string TrapSpell::getDescription() const {
    return "Places a hidden trap that deals damage";
}