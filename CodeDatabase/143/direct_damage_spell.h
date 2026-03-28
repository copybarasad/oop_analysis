#ifndef DIRECT_DAMAGE_SPELL_H
#define DIRECT_DAMAGE_SPELL_H

#include "damage_spell.h"

class DirectDamageSpell : public DamageSpell {
public:
    DirectDamageSpell(const std::string& name,
        const std::string& description,
        int damage,
        int range,
        GameController* gameController);

    void save(std::ostream& out) const override;
    void load(std::istream& in) override;
    uint8_t getType() const;

private:
    bool applyEffect() override;
};

#endif
