#include "area_damage_spell.h"

#include "player.h"
#include "field.h"
#include "enemy.h"
#include "enemy_spawner.h"
#include <iostream>


AreaDamageSpell::AreaDamageSpell(std::string name, int damage) :
    name(std::move(name)), damage(damage) {}


std::string AreaDamageSpell::get_name() const {
    return name;
}


// FIXED
void AreaDamageSpell::cast(Player &caster, GameState &game_state) {
    std::cout << "Casting " << name << "!\n";

    Position center = caster.get_position();
    int hit_count = 0;

    // вынесли логику для избежания дублирования
    auto damage_entity = [&](Entity& entity, Position& pos) {
        if (entity.is_alive() && entity.get_position() == pos) {
            int initial_health = entity.get_health();
            entity.take_damage(damage);
            std::cout << "Hit " << entity.get_name() << " at position (" << pos.get_x() << "," << pos.get_y()
                      << ") for " << damage << " damage! ";
            std::cout << "Health: " << initial_health << " -> " << entity.get_health() << "\n";
            hit_count++;

            if (!entity.is_alive()) {
                std::cout << entity.get_name() << " destroyed!\n";
            }
        }
    };

    // Бьем область 2x2 вокруг центра (вокруг самого игрока)
    for (int dx = -2; dx <= 2; ++dx) {
        for (int dy = -2; dy <= 2; ++dy) {
            Position target_pos(center.get_x() + dx, center.get_y() + dy);

            if (!game_state.get_field().position_is_valid(target_pos)) continue;

            for (Enemy& enemy : game_state.get_enemies()) damage_entity(enemy, target_pos);   // теперь for выглядят красивее
            for (EnemySpawner& spawner : game_state.get_spawners()) damage_entity(spawner, target_pos);
            for (EnemyTower& tower : game_state.get_towers()) damage_entity(tower, target_pos);

        }
    }

    if (hit_count == 0) {
        std::cout << "Area damage spell hit no targets, but area was damaged!\n";
    } else {
        std::cout << "Area damage spell hit " << hit_count << " targets!\n";
    }
}


