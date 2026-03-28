#ifndef SPELL_HPP
#define SPELL_HPP

#include <string>

class Game;

class Spell{
public:
    virtual ~Spell() = default;

    virtual std::string GetName() const noexcept = 0;
    virtual bool UseSpell(Game& game) = 0;
};

#endif