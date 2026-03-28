#include "player_hand.h"
#include <iostream>

#include "area_damage_spell.h"
#include "direct_damage_spell.h"
#include "position.h"
#include "trap_spell.h"
#include "game_state.h"

SpellHand::SpellHand(size_t max_size) : max_size(max_size) {}

SpellHand::~SpellHand() {                   // освобождаем память!!!
    clear_spells();
}

void SpellHand::add_spell(ISpell* spell) {
    if (spells.size() < max_size) {
        spells.push_back(spell);
        std::cout << "добавлено новое заклинание " << spell->get_name() << std::endl ;
    } else {
        delete spell; // если рука полная - удаляем это заклинание (очищаем выделенную при создании заклинания память), и не добавляем его
    }
}


void SpellHand::cast_spell(int index, Player& caster, GameState& game_state) {

    spells[index]->cast(caster, game_state); // закастили заклинание

    delete spells[index];
    spells.erase(spells.begin() + index);

}


size_t SpellHand::get_spell_count() const {
    return spells.size();
}


std::string SpellHand::get_spell_name(size_t index) const {
    return spells[index]->get_name();
}


void SpellHand::clear_spells() {    // освобождаем память!!!
    for (auto* spell : spells) {
        delete spell;
    }
    spells.clear();
}


// NEW METHODS IMPLEMENTATION
void SpellHand::load_spells_from_names(const std::vector<std::string>& spell_names) {
    // Очищаем текущие заклинания
    clear_spells();

    for (const auto& name : spell_names) {
        ISpell* spell = nullptr;

        // Создаем заклинания со стандартными параметрами
        if (name == "Fireball") {
            spell = new DirectDamageSpell("Fireball", 20, 3);
        }
        else if (name == "Firestorm") {
            spell = new AreaDamageSpell("Firestorm", 15);
        }
        else if (name == "Bear Trap") {
            spell = new TrapSpell("Bear Trap", 25);
        }
        else if (name == "Ranged kick") {
            spell = new DirectDamageSpell("Ranged kick", 15, 4);
        }

        if (spell) {
            add_spell(spell);
        }
    }
}


// FIXED (for loops)
bool SpellHand::can_cast_spell(int index, const Position& player_pos, GameState& game_state) const {

    // for direct_damage, for other true always
    if (get_spell_name(index) == "Ranged kick" or get_spell_name(index) == "Fireball") {
        // Можно тоже свести к game_state.find_nearest_target(), но там гарантированный перебор всех (нужно для минимизации без return), а тут мы выходим из цикла, если нашли кого-то
        int range = (get_spell_name(index) == "Ranged kick") ? 3 : 2;

        // во избежание дублирования кода выносим логику в лямбда выражение
        auto has_entity_in_range = [&](Entity& entity) {
            return entity.is_alive() && player_pos.find_distance_to(entity.get_position()) <= range;
        };


        for (Enemy& enemy : game_state.get_enemies())
            if (has_entity_in_range(enemy)) return true;

        for (EnemySpawner& spawner : game_state.get_spawners())
            if (has_entity_in_range(spawner)) return true;

        for (EnemyTower& tower : game_state.get_towers())
            if (has_entity_in_range(tower)) return true;

        return false;

    }

    return true;
}
