#ifndef DIRECTDAMAGESPELL_H
#define DIRECTDAMAGESPELL_H

#include "Spell.h"

class DirectDamageSpell : public ISpell{
private:
    int range_;
    int damage_;

    int Chebyshev(std::pair<int,int> a, std::pair<int,int> b) const;

public:
    DirectDamageSpell(int range, int damage);

    virtual const char* GetName() const;
    virtual bool NeedsTarget() const;
    virtual int GetRange() const;

    virtual bool CanCast(SpellContext& context);
    virtual bool Cast(SpellContext& context);

    virtual bool ConsumesModifier() const;
    virtual bool IsPersistent() const;

    virtual void ApplyModifier(const SpellModifier& mod);
    virtual ISpell* CloneNew() const;

    void IncreaseDamage(int delta) { damage_ += delta; }
};

#endif