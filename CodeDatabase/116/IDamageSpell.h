#pragma once
#include "ISpell.h"

class DamageSpellContext;
class EnemySpellContext;

class IDamageSpell : public ISpell {
public:
    virtual ~IDamageSpell() = default;
    virtual void apply(DamageSpellContext& context) = 0;
    virtual void apply(EnemySpellContext& context) = 0;
    virtual int getDamage() = 0;
};