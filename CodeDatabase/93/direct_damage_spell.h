#ifndef DIRECT_DAMAGE_SPELL_H
#define DIRECT_DAMAGE_SPELL_H

#include "spell.h"

class DirectDamageSpell : public Spell {
private:
    int damage_;
    int range_;

public:
    DirectDamageSpell(int damage = 25, int range = 3);
    bool use(GameController& game, int x, int y) override;
    std::string getName() const override;
    std::string getDescription() const override;

    void save(std::ostream& os) const override;
    void load(std::istream& is) override;

    void upgrade() override;
};

#endif