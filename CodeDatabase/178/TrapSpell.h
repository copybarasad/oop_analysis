#ifndef TRAP_SPELL_H
#define TRAP_SPELL_H

#include "ISpell.h"

class TrapSpell : public ISpell
{
private:
    int damage;
    
public:
    TrapSpell(int dmg = 8);
    bool apply(GameController &controller, const Position &casterPosition, const Position &targetPos) override;
    const char *name() const override { return "Trap"; }
    SpellType getType() const override { return SpellType::TRAP; }
    void getParameters(int& dmg, int& range, int& width, int& height, int& count, int& power) const override {
        dmg = damage;
        range = 0;
        width = 0;
        height = 0;
        count = 0;
        power = 0;
    }
    void increaseDamage(int d) { damage += d; }
    
    int getDamage() const { return damage; }
};

#endif