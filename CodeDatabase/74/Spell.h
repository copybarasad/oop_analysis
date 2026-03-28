#ifndef SPELL_H
#define SPELL_H

#include "Types.h"
#include "Serialization.h"
#include <string>

class Spell {
protected:
    int damage;
public:
    Spell(int damage);
    virtual ~Spell() = default;

    virtual CastInfo getCastInfo() const = 0;

    int getDamage() const {
        return damage;
    }

    virtual std::string getType() const = 0;

    virtual json serialize() const = 0;
    virtual void deserialize(const json& j) = 0;
};

#endif // SPELL_H