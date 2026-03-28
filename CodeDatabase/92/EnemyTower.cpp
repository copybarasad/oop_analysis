#include "EnemyTower.h"

void EnemyTower::attack(Game &game, std::pair<int, int> target) {
    if (current_delay != 0) {
        current_delay--;
        return;
    }

    if (!spell.checkTarget(position, target)) return;
    
    spell.use(game, position, target);
    current_delay = delay_turns;
}

json EnemyTower::to_json() {
    json j = Entity::to_json();
    j["delay_turns"] = delay_turns;
    j["current_delay"] = current_delay;
    j["spell_range"] = spell.get_range();
    return j;
}

bool EnemyTower::from_json(json &j) {
    if (!Entity::from_json(j)) return false;
    try {
        delay_turns = j.at("delay_turns");
        current_delay = j.at("current_delay");
        int rng = j.at("spell_range");
        spell = DirectSpell((int)damage, rng, Spell::Type::Direct);
        return true;
    } catch (...) { return false; }
}
