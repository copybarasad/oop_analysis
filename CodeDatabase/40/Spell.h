#ifndef SPELL_H
#define SPELL_H
#include "Character.h"
#include "Field.h"

#include <string>

class Game;

class Spell{
protected:
    int id;
    std::string name;
    int cost;

public:
    Spell(int id, const std::string& name, int cost);
    virtual ~Spell() = default;

    int getId() const;
    std::string getName() const;
    int getCost() const;

    virtual bool isAreaSpell() const = 0;
    virtual void apply(Character& caster, Character& target) const = 0;
    virtual void applyArea(Character& caster, int centerX, int centerY, Game& game) = 0;
};

#endif