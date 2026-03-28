#include "SummonSpell.h"
#include "GameController.h"
#include "Logger.h"

bool SummonSpell::cast(Player& caster, GameController& game) {
    int alliesToSummon = 1;

    int buffCharges = caster.getBuffCharges();
    if (buffCharges > 0) {
        alliesToSummon += buffCharges;
        game.logEvent(LogMsg::SPELL_BUFF);
        caster.consumeBuffCharges();
    }
    
    int summonedCount = 0;
    for (int i = 0; i < alliesToSummon; ++i) {
        if (game.spawnAllyNear(caster.getX(), caster.getY())) {
            summonedCount++;
        }
    }
    
    if (summonedCount > 0) {
        game.logEvent(LogMsg::SPELL_SUMMON);
    } else {
        game.logEvent(LogMsg::SPELL_SUMMON_FAIL);
    }

    return true;
}

std::string SummonSpell::getName() const { 
    return "Summon Ally";
}

std::string SummonSpell::getDescription() const { 
    return "Summons an ally to fight for you."; 

}