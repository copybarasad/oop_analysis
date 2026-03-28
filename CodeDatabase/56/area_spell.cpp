#include "area_spell.h"


AreaSpell::AreaSpell(int spell_type, std::string spell_name, int cost, int x, int y, int d, int r)
    : Spell(spell_type, spell_name, cost, d), 
      area_x(x), area_y(y), range(r) {
}
int AreaSpell::getAreaX() const { return area_x; }
int AreaSpell::getAreaY() const { return area_y; }
int AreaSpell::getDamage() const { return damage; }
int AreaSpell::getRange() const { return range; }

nlohmann::json AreaSpell::toJson() const {
    auto j = Spell::toJson();
    j["area_x"] = area_x;
    j["area_y"] = area_y;
    j["damage"] = damage;
    j["range"] = range;
    return j;
}
void AreaSpell::setDamage(int d){
    damage = d;
}
std::unique_ptr<Spell> AreaSpell::clone() const {
    return std::make_unique<AreaSpell>(*this);
}
void AreaSpell::fromJson(const nlohmann::json& j) {
    Spell::fromJson(j);
    area_x = j["area_x"];
    area_y = j["area_y"];
    damage = j["damage"];
    range = j["range"];
}
