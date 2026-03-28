#ifndef SPELL_H
#define SPELL_H

#include <string>
#include <memory>
#include <iostream>

class GameController;

class Spell {
public:
    virtual ~Spell() = default;
    virtual bool use(GameController& game, int x, int y) = 0;
    virtual std::string getName() const = 0;
    virtual std::string getDescription() const = 0;

    virtual void save(std::ostream& os) const = 0;
    virtual void load(std::istream& is) = 0;

    virtual void upgrade() = 0;
};

using SpellPtr = std::unique_ptr<Spell>;

#endif 