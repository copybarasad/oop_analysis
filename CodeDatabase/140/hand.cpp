#include "hand.h"
#include "direct_spell.h"
#include "area_spell.h"
#include "trap_spell.h"
#include "create_spell.h"
#include "levelup_spell.h"
#include "field.h"

bool Hand::add_spell(unique_ptr<Spell> spell) {
    if (spells.size() < max_size) {
        spells.push_back(move(spell));
        return true;
    }
    return false;
}

void Hand::use_spell(size_t idx, Field& field, int caster_x, int caster_y, int target_x, int target_y) {
    if (idx >= spells.size() || !spells[idx]) return;

    Player* player = field.get_player();
    int level = player ? player->get_level() : 0;

    unique_ptr<Spell> spell = unique_ptr<Spell>(spells[idx]->clone());

    if (level > 0) {
        if (auto* direct = dynamic_cast<DirectDamageSpell*>(spell.get())) {
            direct->set_radius(direct->get_radius() + level);
            cout << "Direct Damage radius + " << level << "!\n";
        }
        else if (auto* area = dynamic_cast<AreaDamageSpell*>(spell.get())) {
            area->set_radius(area->get_radius() + level);
            cout << "Area Damage radius + " << level << "!\n";
        }
        else if (auto* trap = dynamic_cast<TrapSpell*>(spell.get())) {
            trap->set_damage(trap->get_damage() + 10 * level);
            cout << "Trap damage + " << (10 * level) << "!\n";
        }
        else if (auto* summon = dynamic_cast<CreateSpell*>(spell.get())) {
            summon->set_ally_count(1 + level);
            cout << "Summon " << (1 + level) << " allies!\n";
        }
        else if (dynamic_cast<LevelupSpell*>(spell.get())) {
            level = 0;
        }

        if (!dynamic_cast<LevelupSpell*>(spell.get())) {
            if (player) player->consume_levels();
        }
    }
    
    spell->use(field, caster_x, caster_y, target_x, target_y);
    spells.erase(spells.begin() + idx);
    cout << "Spell consumed and removed from hand!\n";
}

const vector<unique_ptr<Spell>>& Hand::get_spells() const {
    return spells;
}

size_t Hand::size() const {
    return spells.size();
}

bool Hand::is_full() const {
    return spells.size() >= max_size;
}