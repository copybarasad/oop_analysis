#include "SummonSpell.h"
#include "GameController.h"

SummonSpell::SummonSpell(int summonCount, int range)
    : summonCount(summonCount), range(range) {
}

SpellResult SummonSpell::cast(GameController& game, const Position& target, EnhancementContext& enhancement) {
    int effectiveSummons = summonCount + enhancement.bonusSummons;
    
    int summoned = game.summonAllies(effectiveSummons);
    
    if (summoned > 0) {
        return SpellResult(true, true, "Призвано союзников: " + std::to_string(summoned));
    }
    
    return SpellResult(false, true, "Нет места для призыва союзников!");
}

std::string SummonSpell::getName() const {
    return "Призыв";
}

std::string SummonSpell::getDescription() const {
    return "Призывает " + std::to_string(summonCount) + " союзника рядом с игроком";
}

char SummonSpell::getSymbol() const {
    return 'U';
}

int SummonSpell::getRange() const {
    return range;
}

std::unique_ptr<Spell> SummonSpell::clone() const {
    return std::make_unique<SummonSpell>(summonCount, range);
}







