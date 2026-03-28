#include "TrapSpell.h"
#include "GameController.h"
#include <cmath>

TrapSpell::TrapSpell(int damage, int range)
    : damage(damage), range(range) {
}

SpellResult TrapSpell::cast(GameController& game, const Position& target, EnhancementContext& enhancement) {
    Position playerPos = game.getPlayer().get_Position();
    int effectiveRange = range;
    int effectiveDamage = damage + enhancement.bonusDamage;

    int dx = std::abs(target.x - playerPos.x);
    int dy = std::abs(target.y - playerPos.y);
    if (dx > effectiveRange || dy > effectiveRange) {
        return SpellResult(false, false, "Цель вне радиуса действия!");
    }

    if (!game.canPlaceTrap(target)) {
        return SpellResult(false, false, "Невозможно поставить ловушку здесь!");
    }

    game.placeTrap(target, effectiveDamage);
    return SpellResult(true, true, "Ловушка установлена!");
}

std::string TrapSpell::getName() const {
    return "Ловушка";
}

std::string TrapSpell::getDescription() const {
    return "Устанавливает ловушку с " + std::to_string(damage) + " урона";
}

char TrapSpell::getSymbol() const {
    return 'T';
}

int TrapSpell::getRange() const {
    return range;
}

std::unique_ptr<Spell> TrapSpell::clone() const {
    return std::make_unique<TrapSpell>(damage, range);
}







