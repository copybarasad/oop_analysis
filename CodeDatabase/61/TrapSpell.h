#ifndef TRAPSPELL_H
#define TRAPSPELL_H

#include "Spell.h"

class Game;

class TrapSpell : public Spell {
private:
    int enhancedDamage;

public:
    TrapSpell(int radius = 3, int trapDamage = 30);
    
    SpellResult use(Game& game, int targetX, int targetY) override;
    std::string getName() const override;
    std::string getDescription() const override;
    void applyEnhancement(int radiusBonus, int damageBonus, int areaBonus, int summonBonus) override;
    std::unique_ptr<Spell> clone() const override;
};

#endif // TRAPSPELL_H

