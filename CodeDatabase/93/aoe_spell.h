#ifndef AOE_SPELL_H
#define AOE_SPELL_H

#include "spell.h"

class AoESpell : public Spell {
private:
    int damage_;

public:
    AoESpell(int damage = 10);
    bool use(GameController& game, int x, int y) override;
    std::string getName() const override;
    std::string getDescription() const override;

    void save(std::ostream& os) const override;
    void load(std::istream& is) override;

    void upgrade() override;
};

#endif 