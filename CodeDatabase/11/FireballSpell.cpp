#include "FireballSpell.h"
#include "GameController.h"
#include "Logger.h"
#include "Player.h"

FireballSpell::FireballSpell(int dmg, int rng) : damage(dmg), range(rng) {}

bool FireballSpell::cast(Player& caster, GameController& game) {
    char command = game.getInputChar("Choose direction for Fireball (w/a/s/d): "); 
    int dx = 0, dy = 0;
    if (command == 'w') dy = -1; else if (command == 's') dy = 1;
    else if (command == 'a') dx = -1; else if (command == 'd') dx = 1;
    else {
        game.logEvent(LogMsg::SPELL_FAIL); 
        return false;
    }

    int currentRange = this->range;
    int buffCharges = caster.getBuffCharges();
    if (buffCharges > 0) {
        currentRange += 2 * buffCharges;
        caster.consumeBuffCharges();
    }

    for (int i = 1; i <= currentRange; ++i) {
        int projX = caster.getX() + i * dx;
        int projY = caster.getY() + i * dy;
        
        if (!game.getField().isValidPosition(projX, projY) || game.getField().getCellType(projX, projY) == CellType::WALL) {
            game.logEvent(LogMsg::SPELL_FIREBALL_WALL);
            return true;
        }
        
        CellType targetType = game.getField().getCellType(projX, projY);
        if (targetType == CellType::ENEMY || targetType == CellType::ENEMY_TOWER || targetType == CellType::SPAWNER) {
            game.damageAt(projX, projY, this->damage); 
            game.logEvent(LogMsg::SPELL_FIREBALL_HIT);
            return true;
        }
    }
    
    return true;
}

bool FireballSpell::castByTower(int startX, int startY, int targetX, int targetY, GameController& game) {
    int dx = (targetX > startX) ? 1 : (targetX < startX) ? -1 : 0;
    int dy = (targetY > startY) ? 1 : (targetY < startY) ? -1 : 0;

    for (int i = 1; i <= this->range; ++i) {
        int projX = startX + i * dx;
        int projY = startY + i * dy;

        if (!game.getField().isValidPosition(projX, projY) || game.getField().getCellType(projX, projY) == CellType::WALL) {
            return false; 
        }
        if (projX == targetX && projY == targetY) {
            game.damagePlayer(this->damage);
            game.logEvent(LogMsg::TOWER_HIT_PLAYER);
            return true;
        }
        if (game.getField().getCellType(projX, projY) != CellType::EMPTY) {
            return false;
        }
    }
    return false;
}

std::string FireballSpell::getName() const { 
    return "Fireball"; 
}

std::string FireballSpell::getDescription() const { 
    return "Deals damage to the first enemy in a line."; 
}