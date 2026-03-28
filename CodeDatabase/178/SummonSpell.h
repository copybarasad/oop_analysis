#ifndef SUMMON_SPELL_H
#define SUMMON_SPELL_H

#include "ISpell.h"

class SummonSpell : public ISpell
{
private:
    int count;
    
public:
    SummonSpell(int cnt = 1);
    bool apply(GameController &controller, const Position &casterPosition, const Position &targetPos) override;
    const char *name() const override { return "Summon"; }
    SpellType getType() const override { return SpellType::SUMMON; }
    void getParameters(int& damage, int& range, int& width, int& height, int& cnt, int& power) const override {
        damage = 0;
        range = 0;
        width = 0;
        height = 0;
        cnt = count;
        power = 0;
    }
    void increaseCount(int d) { count += d; }
    
    int getCount() const { return count; }
};

#endif