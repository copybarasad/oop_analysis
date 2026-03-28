#include "trap_spell.h"
#include "player.h"
#include "enemy_manager.h"
#include "game_field.h"
#include "trap.h"

void TrapSpell::Cast(Player& player, EnemyManager& enemies, GameField& field) {
    Trap::CastTrapSpell(player, enemies, field);
}