#ifndef SPELL_H
#define SPELL_H

#include <string>

enum class SpellType {
    SINGLE_TARGET,
    AREA_EFFECT
};

class Spell {
protected:
    std::string name;
    std::string description;
    int range;
    SpellType type;

public:
    Spell(const std::string& name, const std::string& description, int range, SpellType type);
    virtual ~Spell() = default;

    std::string getName() const;
    std::string getDescription() const;
    int getRange() const;
    SpellType getType() const;
};

#endif