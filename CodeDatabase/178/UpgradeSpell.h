#ifndef UPGRADE_SPELL_H
#define UPGRADE_SPELL_H

#include "ISpell.h"

class UpgradeSpell : public ISpell
{
private:
    int power;

public:
    UpgradeSpell(int p = 1);
    bool apply(GameController &controller, const Position &casterPosition, const Position &targetPos) override;
    const char *name() const override { return "Upgrade"; }
    SpellType getType() const override { return SpellType::UPGRADE; }
    void getParameters(int& damage, int& range, int& width, int& height, int& count, int& pwr) const override {
        damage = 0;
        range = 0;
        width = 0;
        height = 0;
        count = 0;
        pwr = power;
    }
    int getPower() const { return power; }
    void increasePower(int amount) { power += amount; }
};

#endif