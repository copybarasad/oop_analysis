#ifndef SUMMONSPELL_H
#define SUMMONSPELL_H

#include "Spell.h"

class SummonSpell : public ISpell{
private:
    int count_;

public:
    SummonSpell(int count);

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