#include "spawn_enemy_spell.hpp"
#include "field_handler.hpp"

SpawnEnemySpell::SpawnEnemySpell(): SpawnSpell('E') {};

bool SpawnEnemySpell::use(FieldHandler& field_handler, const Position& from, const Position& target) {
    Enemy* en = new Enemy();
    field_handler.place_obj(target, en);

    return true;
}