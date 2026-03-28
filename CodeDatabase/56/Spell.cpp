#include "Spell.h"



Spell::Spell(int spell_type, std::string spell_name, int cost, int dmg){
    type = spell_type;
    name = spell_name;
    mana_cost = cost;
    damage = dmg;
}
std::string Spell::getName() { return name; }
int Spell::getManaCost() { return mana_cost; }
int Spell::getType() { return type; }

nlohmann::json Spell::toJson() const {
    return {
        {"type", type},
        {"name", name},
        {"mana_cost", mana_cost},
        {"damage", damage}
    };
}

void Spell::fromJson(const nlohmann::json& j) {
    type = j["type"];
    name = j["name"];
    mana_cost = j["mana_cost"];
    damage = j["damage"];
}