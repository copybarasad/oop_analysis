#include "DirectDamageSpell.h"
#include "GameController.h"
#include <cmath>

DirectDamageSpell::DirectDamageSpell(int damage, int range)
    : damage(damage), range(range) {
}

SpellResult DirectDamageSpell::cast(GameController& game, const Position& target, EnhancementContext& enhancement) {
    Position playerPos = game.getPlayer().get_Position();
    int effectiveRange = range + enhancement.bonusRange;
    int effectiveDamage = damage + enhancement.bonusDamage;

    int dx = std::abs(target.x - playerPos.x);
    int dy = std::abs(target.y - playerPos.y);
    if (dx > effectiveRange || dy > effectiveRange) {
        return SpellResult(false, false, "Цель вне радиуса действия!");
    }

    if (game.damageEnemyAt(target, effectiveDamage)) {
        return SpellResult(true, true, "Прямой урон нанесён врагу!");
    }

    if (game.damageBuildingAt(target, effectiveDamage)) {
        return SpellResult(true, true, "Прямой урон нанесён зданию!");
    }

    if (game.damageTowerAt(target, effectiveDamage)) {
        return SpellResult(true, true, "Прямой урон нанесён башне!");
    }

    return SpellResult(false, false, "Нет цели для атаки!");
}

std::string DirectDamageSpell::getName() const {
    return "Прямой урон";
}

std::string DirectDamageSpell::getDescription() const {
    return "Наносит " + std::to_string(damage) + " урона врагу или зданию в радиусе " + std::to_string(range);
}

char DirectDamageSpell::getSymbol() const {
    return 'D';
}

int DirectDamageSpell::getRange() const {
    return range;
}

std::unique_ptr<Spell> DirectDamageSpell::clone() const {
    return std::make_unique<DirectDamageSpell>(damage, range);
}







