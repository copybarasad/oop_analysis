#ifndef SUMMONSPELL_H
#define SUMMONSPELL_H

#include "Spell.h"

class Game;

class SummonSpell : public Spell {
private:
    int enhancedSummonCount;

public:
    SummonSpell(int radius = 1, int summonCount = 1);
    
    SpellResult use(Game& game, int targetX, int targetY) override;
    std::string getName() const override;
    std::string getDescription() const override;
    void applyEnhancement(int radiusBonus, int damageBonus, int areaBonus, int summonBonus) override;
    std::unique_ptr<Spell> clone() const override;
};

#endif // SUMMONSPELL_H

