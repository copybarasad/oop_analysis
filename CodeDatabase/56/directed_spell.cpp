#include "directed_spell.h"

DirectedSpell::DirectedSpell(int spell_type, std::string spell_name, int cost, int d, int r)
    : Spell(spell_type, spell_name, cost, d),
     range(r) {
}
int DirectedSpell::getDamage() const { return damage;}
int DirectedSpell::getRange(){ return range; }

nlohmann::json DirectedSpell::toJson() const {
    auto j = Spell::toJson();
    j["damage"] = damage;
    j["range"] = range;
    return j;
}

std::unique_ptr<Spell> DirectedSpell::clone() const {
    return std::make_unique<DirectedSpell>(*this);
}

void DirectedSpell::fromJson(const nlohmann::json& j) {
    Spell::fromJson(j);
    damage = j["damage"];
    range = j["range"];
}
void DirectedSpell::setDamage(int d){
    damage = d;
}