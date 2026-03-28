#pragma once
#include <string>
#include "Coordinates.h"

class BasicSpell {
public:
    enum class Type {Direct, AoE, Trap};
protected:
    int damage;
    std::string name;
    Type type;
    float range;
public:
    BasicSpell(Type type, int damage, float range, std::string name) :
    damage(damage), name(name), type(type), range(range) {}

    virtual ~BasicSpell() = default;
    int getDamage(){ return damage; }
    std::string getName(){ return name; }
    Type getType(){ return type;
}
    bool doesSpellReach(Coordinates caster_pos, Coordinates target_pos) {
        return Coordinates::distanceBetween(caster_pos, target_pos) <= static_cast<float>(range);
}
    std::string toString();

    float getRange() { return range; }

    virtual json toJson() const;
    virtual void fromJson(const json& j);
};