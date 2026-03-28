#ifndef SPLASH_SPELL_H
#define SPLASH_SPELL_H

#include "Spell.h"
#include "Types.h"
#include <string>

class SplashSpell : public Spell {
    int distance;
    int radius;
public:
    SplashSpell(int damage, int distance, int radius)
        : Spell(damage), distance(distance), radius(radius) {
    }

    CastInfo getCastInfo() const override {
        CastInfo info;
        info.type = SpellType::Splash;
        info.damage = damage;
        info.distance = distance;
        info.radius = radius;
        info.pos = { -1, -1 };
        return info;
    }

    std::string getType() const override {
        return "Splash";
    };

    json serialize() const {
        json j;
        j["type"] = "Splash";
        j["damage"] = damage;
        j["distance"] = distance;
        j["radius"] = radius;
        return j;
    }

    void deserialize(const json& j) {
        damage = j.at("damage").get<int>();
        distance = j.at("distance").get<int>();
        radius = j.at("radius").get<int>();
    }
};

#endif // SPLASH_SPELL_H
