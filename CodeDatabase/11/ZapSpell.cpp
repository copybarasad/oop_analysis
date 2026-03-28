#include "ZapSpell.h"
#include "GameController.h"
#include "Logger.h"
#include <cmath>

bool ZapSpell::cast(Player& caster, GameController& game) {
    int targetX = game.getInputInt("Enter target X coordinate for Zap: ");
    int targetY = game.getInputInt("Enter target Y coordinate for Zap: ");

    if (std::abs(caster.getX() - targetX) > 10 || std::abs(caster.getY() - targetY) > 10) {
        game.logEvent(LogMsg::SPELL_FAIL);
        return false;
    }
    int areaSize = 2;
    int damage = 30;

    int buffCharges = caster.getBuffCharges();
    if (buffCharges > 0) {
        areaSize += buffCharges;
        game.logEvent(LogMsg::SPELL_BUFF);
        caster.consumeBuffCharges();
    }

    game.logEvent(LogMsg::SPELL_ZAP_HIT);
    for (int y = targetY; y < targetY + areaSize; ++y) {
        for (int x = targetX; x < targetX + areaSize; ++x) {
             game.damageAt(x, y, damage);
        }
    }
    return true;
}

std::string ZapSpell::getName() const { 
    return "Zap"; 
}

std::string ZapSpell::getDescription() const { 
    return "Deals damage to enemies/towers in area."; 
}
