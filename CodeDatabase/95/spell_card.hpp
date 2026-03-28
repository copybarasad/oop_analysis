#ifndef SPELL_CARD_HPP
#define SPELL_CARD_HPP

#include <string>

class SpellCard {
public:
    virtual ~SpellCard() = default;

    virtual std::string getName() const = 0;

    virtual int getRange() const = 0;
};

#endif