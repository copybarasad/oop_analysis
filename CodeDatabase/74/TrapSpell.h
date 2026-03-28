#ifndef TRAP_SPELL_H
#define TRAP_SPELL_H

#include "Spell.h"
#include "Types.h"
#include <string>

class TrapSpell : public Spell {
    Coords pos;
public:
    explicit TrapSpell(int damage, Coords pos = { -1, -1 })
        : Spell(damage), pos(pos) {
    }

    CastInfo getCastInfo() const override {
        CastInfo info;
        info.type = SpellType::Trap;
        info.damage = damage;
        info.distance = 0;
        info.radius = 0;
        info.pos = pos;
        return info;
    }

    std::string getType() const override {
        return "Trap";
    };

    json serialize() const {
        json j;
        j["type"] = "Trap";
        j["damage"] = damage;
        j["pos"] = { pos.x, pos.y };
        return j;
    }

    void deserialize(const json& j) {
        damage = j.at("damage").get<int>();
        auto arr = j.at("pos");
        pos.x = arr[0].get<int>();
        pos.y = arr[1].get<int>();
    }
};

#endif // TRAP_SPELL_H
