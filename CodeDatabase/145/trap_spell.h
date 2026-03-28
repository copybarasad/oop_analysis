#ifndef TRAP_SPELL_H
#define TRAP_SPELL_H

#include "spell.h"
#include "trap_context.h"
#include <string>

class TrapSpell : public Spell {
private:
    int damage;
    int radius;
    
public:
    TrapSpell(int damage = 15, int radius = 4);
    bool apply(const TrapContext& context);
    bool canUse(const TrapContext& context) const;
    const char* getName() const override;
    std::unique_ptr<Spell> clone() const override;
    void applyEnhancement() override;
    std::string getSerializedData() const override;
    int getDamage() const;
    void setDamage(int newDamage);
};

#endif