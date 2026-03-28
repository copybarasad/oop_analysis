#include "SpellTrap.hpp"
#include <iostream>
#include <cmath>
#include "SpellEnhancement.hpp"

int SpellTrap::trap_damage = 3;
SpellTrap::SpellTrap() {
    range = 4;
    mana_cost = 5;
}

int SpellTrap::get_trap_damage() {
    return trap_damage;
}

int SpellTrap::get_range() const {
    return range;
}

int SpellTrap::get_mana_cost() const {
    return mana_cost;
}

bool SpellTrap::requires_target() const {
    return true;
}

std::string SpellTrap::get_name() const {
    return "Trap";
}

bool SpellTrap::cast(Player& player, Board& board, std::vector<Enemy>& enemies,
                      std::vector<Ally>& allies,
                      EnemyAttackTower& attack_tower, EnemyTower& tower,
                      int targetX, int targetY) {

    std::cout << "=== TRAP SPELL DEBUG ===" << std::endl;
    std::cout << "Attempting to place trap at: (" << targetX << ", " << targetY << ")" << std::endl;

    if (SpellEnhancement::has_enhancement()) {
        int enhanced_damage = SpellEnhancement::apply_trap_enhancement(trap_damage);
        trap_damage = enhanced_damage;
    }

    int playerX, playerY;
    player.get_coords(playerX, playerY);
    std::cout << "Player position: (" << playerX << ", " << playerY << ")" << std::endl;

    int distance = std::abs(targetX - playerX) + std::abs(targetY - playerY);
    if (distance > range) {
        std::cout << "Target area is too far away. Distance: " << distance << ", Range: " << range << std::endl;
        return false;
    }

    if (!board.can_move_to(targetX, targetY)) {
        std::cout << "Cannot place trap - cell not movable" << std::endl;
        return false;
    }

    Cell& cell = board.get_cell(targetX, targetY);
    std::cout << "Cell at (" << targetX << ", " << targetY << ") status: "
              << "player=" << cell.is_player_here()
              << ", enemy=" << cell.is_enemy_here()
              << ", trap=" << cell.is_trap_here()
              << ", obstacle=" << cell.is_obstacle_here() << std::endl;

    if (cell.is_player_here() || cell.is_enemy_here() || cell.is_trap_here()) {
        std::cout << "Target cell is occupied" << std::endl;
        return false;
    }

    cell.set_trap(true);
    std::cout << "SUCCESS: Trap placed at (" << targetX << ", " << targetY << ")" << std::endl;
    std::cout << "=== TRAP SPELL END ===" << std::endl;

    return true;
}