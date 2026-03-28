#include "EnhancementSpell.h"
#include "GameController.h"

SpellResult EnhancementSpell::cast(GameController& game, const Position& target, EnhancementContext& enhancement) {
    enhancement.addStack();
    return SpellResult(true, true, "Улучшение применено! Уровень: " + std::to_string(enhancement.stackCount));
}

std::string EnhancementSpell::getName() const {
    return "Улучшение";
}

std::string EnhancementSpell::getDescription() const {
    return "Усиливает следующее заклинание";
}

char EnhancementSpell::getSymbol() const {
    return 'E';
}

int EnhancementSpell::getRange() const {
    return 0;
}

std::unique_ptr<Spell> EnhancementSpell::clone() const {
    return std::make_unique<EnhancementSpell>();
}







