#include "level.hpp"
#include "hero.hpp"
#include <iostream>

Level::Level(int level_num, int enemy_count, int width, int height)
    : number(level_num), roster(enemy_count, level_num, width, height),
      field(roster, width, height) {
}
bool Level::is_complete() const {
    return roster.enemies.empty() && roster.towers.empty() && roster.attack_towers.empty();
}

void Level::run_turn(Hero& player) {
    roster.move_tower(field.box);
    roster.move_attack_towers(field, roster, player);
    roster.move_mob(field.box, player, field.x, field.y);
    roster.move_ally(field.box, field.x, field.y);
    clear_dead_entities();
    update_entities();
}

void Level::clear_dead_entities() {
    field.remove_dead_entities(roster);
}

void Level::update_entities() {
    roster.update();
}