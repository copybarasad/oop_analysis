#include "spell_trap.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "gamefield.hpp"
#include "game_controller.hpp"

bool Trap_spell::is_valid_target(Position& player_pos, Position& target_pos) const{
    int dx = std::abs(target_pos.get_x() - player_pos.get_x());
    int dy = std::abs(target_pos.get_y() - player_pos.get_y());
    return dx <= radius && dy <= radius;
}

void Trap_spell::apply_effect(Player& player, Position& target_pos, std::vector<std::unique_ptr<Enemy>>& enemies, Gamefield& field){}