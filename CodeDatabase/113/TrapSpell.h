#ifndef TRAPSPELL_H
#define TRAPSPELL_H

#include "Spell.h"
#include "ITrapSpell.h"

class TrapSpell : public Spell, public ITrapSpell {
private:
    int m_damage;

public:
    TrapSpell(const std::string& name, int manaCost, int range, int damage);

    bool cast(Player& caster, GameField& field) override;
    std::string getDescription() const override;

    int getTrapDamage() const override;
    bool canPlaceTrap(const Player& caster, const GameField& field, int x, int y) const override;
    void onTrapTriggered(int x, int y, GameObject& trigger) const override;
    std::string getTrapType() const override;
};

#endif // TRAPSPELL_H