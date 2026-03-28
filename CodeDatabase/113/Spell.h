#ifndef SPELL_H
#define SPELL_H

#include <string>
#include "ISpellCard.h"

class Spell : public ISpellCard {
protected:
    std::string m_name;
    int m_manaCost;
    int m_range;

public:
    Spell(const std::string& name, int manaCost, int range);
    virtual ~Spell() = default;

    const std::string& getName() const override;
    int getManaCost() const override;
    int getRange() const override;

    virtual bool cast(class Player& caster, class GameField& field) = 0;
    virtual std::string getDescription() const override = 0;
};

#endif // SPELL_H