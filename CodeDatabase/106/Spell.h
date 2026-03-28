#ifndef SPELL_H
#define SPELL_H

#include <string>

class Spell {
public:
    virtual ~Spell() = default;

    virtual std::string getName() const = 0;
    virtual int getDamage() const = 0;
    virtual int getRange() const = 0;
    virtual bool getIsAOE() const = 0;
};

#endif // SPELL_H