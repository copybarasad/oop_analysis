#include "buff_spell.h"
#include "field.h"

BuffSpell::BuffSpell(int extraRange, int extraArea, int extraTrapDamage, int extraSummons)
: extraRange_(extraRange), extraArea_(extraArea), extraTrapDamage_(extraTrapDamage), extraSummons_(extraSummons) {}

bool BuffSpell::apply(int casterId, Field& field, int targetRow, int targetCol) {
    Player* p = field.getPlayerById(casterId);
    if (!p) return false;
    
    int currentExtraRange = p->consumeExtraRange();
    int currentExtraArea = p->consumeExtraArea();
    int currentExtraTrapDamage = p->consumeExtraTrapDamage();
    int currentExtraSummons = p->consumeExtraSummons();
    
    p->storeBuffForNextSpell(
        currentExtraRange + extraRange_,
        currentExtraArea + extraArea_,
        currentExtraTrapDamage + extraTrapDamage_,
        currentExtraSummons + extraSummons_
    );
    
    return true;
}

std::string BuffSpell::getName() const { return "Buff"; }
