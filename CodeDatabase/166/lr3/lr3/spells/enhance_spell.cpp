#include "enhance_spell.hpp"

bool EnhanceSpell::cast(Hero&,
                        Field&,
                        std::vector<Enemy>&,
                        std::vector<Spawner>&,
                        BoostState& boosts,
                        const Position&) {
    // Простое суммирование улучшений (как просил): без проверок и лимитов
    boosts.directRadiusPlus += dPlus_;
    boosts.aoeSizePlus      += aPlus_;
    boosts.trapDamagePlus   += tPlus_;
    boosts.summonCountPlus  += sPlus_;
    boosts.stacks++;
    return true;
}
