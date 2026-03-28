#ifndef SPELLAOE_H
#define SPELLAOE_H

#include "Spell.h"

class AoESpell : public ISpell{
private:
    int range_;
    int damage_;
    int size_;

    int Chebyshev(std::pair<int,int> a, std::pair<int,int> b) const;

public:
    AoESpell(int range, int damage, int size);

    virtual const char* GetName() const;
    virtual bool NeedsTarget() const;
    virtual int GetRange() const;

    virtual bool CanCast(SpellContext& context);
    virtual bool Cast(SpellContext& context);

    virtual void ApplyModifier(const SpellModifier& mod);
    virtual bool ConsumesModifier() const;
    virtual ISpell* CloneNew() const;
};

#endif