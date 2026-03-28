#ifndef TRAP_SPELL_H
#define TRAP_SPELL_H

#include "spell.h"

class TrapSpell : public Spell {
private:
    int damage_;

public:
    TrapSpell(int damage = 15);
    bool use(GameController& game, int x, int y) override;
    std::string getName() const override;
    std::string getDescription() const override;

    void save(std::ostream& os) const override;
    void load(std::istream& is) override;

    void upgrade() override;
};

#endif