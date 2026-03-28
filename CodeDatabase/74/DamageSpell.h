#ifndef DAMAGE_SPELL_H
#define DAMAGE_SPELL_H

#include "Spell.h"
#include "Types.h"
#include <string>

class DamageSpell : public Spell {
    int distance;
public:
    DamageSpell(int damage, int distance)
        : Spell(damage), distance(distance) {
    }

    CastInfo getCastInfo() const override {
        CastInfo info;
        info.type = SpellType::Damage;
        info.damage = damage;
        info.distance = distance;
        info.radius = 0;
        info.pos = { -1, -1 };
        return info;
    }

    std::string getType() const override {
        return "Damage";
    };

    json serialize() const {
        json j;
        j["type"] = "Damage";
        j["damage"] = damage;
        j["distance"] = distance;
        return j;
    }

    void deserialize(const json& j) {
        damage = j.at("damage").get<int>();
        distance = j.at("distance").get<int>();
    }
};

#endif // DAMAGE_SPELL_H
