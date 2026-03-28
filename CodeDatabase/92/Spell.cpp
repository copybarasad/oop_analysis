#include "Spell.h"
#include "Game.h"
#include <cmath>
#include "AreaSpell.h"
#include "TrapSpell.h"

std::string Spell::get_type_str() {
    return "Base Spell";
}

bool Spell::use(Game& game, std::pair<int, int> caster, std::pair<int, int> target) {
    return false;
}

bool Spell::checkTarget(std::pair<int, int> caster, std::pair<int, int> target) {
    int dx = caster.first - target.first;
    int dy = caster.second - target.second;
    float distance = std::sqrt(dx * dx + dy * dy);
    return distance <= range;
}

json Spell::to_json() {
    return json{
            {"type_id", static_cast<int>(type)},
            {"damage", damage},
            {"range", range}
    };
}

Spell* Spell::from_json(json& j) {
    try {
        int t_int = j.at("type_id");
        int dmg = j.at("damage");
        int rng = j.at("range");
        Type t = static_cast<Type>(t_int);
        switch (t) {
            case Type::Direct: return new DirectSpell(dmg, rng, t);
            case Type::Area: return new AreaSpell(dmg, rng, t);
            case Type::Trap: return new TrapSpell(dmg, rng, t);
            default: return nullptr;
        }
    } catch (...) { return nullptr; }
}
