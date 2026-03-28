#include "AreaDamageSpell.h"
#include "GameController.h"
#include <cmath>

AreaDamageSpell::AreaDamageSpell(int damage, int range, int areaSize)
    : damage(damage), range(range), areaSize(areaSize) {
}

SpellResult AreaDamageSpell::cast(GameController& game, const Position& target, EnhancementContext& enhancement) {
    Position playerPos = game.getPlayer().get_Position();
    int effectiveRange = range;
    int effectiveArea = areaSize + enhancement.bonusArea;
    int effectiveDamage = damage + enhancement.bonusDamage;

    int dx = std::abs(target.x - playerPos.x);
    int dy = std::abs(target.y - playerPos.y);
    if (dx > effectiveRange || dy > effectiveRange) {
        return SpellResult(false, false, "Цель вне радиуса действия!");
    }

    if (!game.getField().isValidPosition(target.x, target.y)) {
        return SpellResult(false, false, "Недопустимая позиция!");
    }

    int hits = 0;
    for (int offsetY = 0; offsetY < effectiveArea; ++offsetY) {
        for (int offsetX = 0; offsetX < effectiveArea; ++offsetX) {
            Position aoeTarget(target.x + offsetX, target.y + offsetY);
            
            if (game.damageEnemyAt(aoeTarget, effectiveDamage)) {
                hits++;
            }
            if (game.damageBuildingAt(aoeTarget, effectiveDamage)) {
                hits++;
            }
            if (game.damageTowerAt(aoeTarget, effectiveDamage)) {
                hits++;
            }
        }
    }

    std::string msg = "Урон по площади " + std::to_string(effectiveArea) + "x" + std::to_string(effectiveArea);
    if (hits > 0) {
        msg += " - поражено целей: " + std::to_string(hits);
    }
    return SpellResult(true, true, msg);
}

std::string AreaDamageSpell::getName() const {
    return "Урон по площади";
}

std::string AreaDamageSpell::getDescription() const {
    return "Наносит " + std::to_string(damage) + " урона в области " + 
           std::to_string(areaSize) + "x" + std::to_string(areaSize);
}

char AreaDamageSpell::getSymbol() const {
    return 'A';
}

int AreaDamageSpell::getRange() const {
    return range;
}

std::unique_ptr<Spell> AreaDamageSpell::clone() const {
    return std::make_unique<AreaDamageSpell>(damage, range, areaSize);
}







