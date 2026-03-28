#include "trap_spell.h"

TrapSpell::TrapSpell(int spell_type, std::string spell_name, int cost, int d, int r)
    : Spell(spell_type, spell_name, cost, d),
      range(r) {
}

int TrapSpell::getDamage() const { return damage; }

int TrapSpell::getRange() { return range; } 

nlohmann::json TrapSpell::toJson() const {
    auto j = Spell::toJson();
    j["damage"] = damage;
    j["range"] = range;
    return j;
}

void TrapSpell::fromJson(const nlohmann::json& j) {
    Spell::fromJson(j);
    damage = j["damage"];
    range = j["range"];
}
std::unique_ptr<Spell> TrapSpell::clone() const {
    return std::make_unique<TrapSpell>(*this);
}
void TrapSpell::setDamage(int d){
    damage = d;
}

