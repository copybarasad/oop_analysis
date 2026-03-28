// Spell.h
#ifndef SPELL_H
#define SPELL_H

#include <string>

struct SpellEnhancement {
    int extraDamage = 0;
    int extraRange = 0;
    int extraArea = 0;
    int extraSummons = 0;
};

class Game;
class Player;

class Spell {
public:
    virtual ~Spell() = default;

    virtual int cost() const = 0;
    virtual bool apply(Game& game, Player& player, int tx, int ty, const SpellEnhancement& enh, std::string* errorMessage = nullptr) = 0;
    virtual const char* name() const = 0;
    virtual std::string getDescription() const = 0;
};

#endif 