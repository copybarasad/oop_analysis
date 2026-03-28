#ifndef EMPOWERSPELL_H
#define EMPOWERSPELL_H

#include "Spell.h"

class EmpowerSpell : public ISpell{
private:
    SpellModifier gain_;

public:
    EmpowerSpell(int rDelta, int areaDelta, int dmgDelta, int summonDelta);

    virtual const char* GetName() const;
    virtual bool NeedsTarget() const;
    virtual int GetRange() const;

    virtual bool CanCast(SpellContext& context);
    virtual bool Cast(SpellContext& context);

    virtual void ApplyModifier(const SpellModifier& mod);
    virtual bool ConsumesModifier() const;
    virtual bool IsPersistent() const;

    virtual ISpell* CloneNew() const;
};

#endif