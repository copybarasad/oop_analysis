#include "trap_spell.h"
#include "player.h"
#include "playing_field.h"
#include "enemy.h"
#include <cmath>
#include <iostream>
#include <random>
#include <algorithm>

TrapSpell::TrapSpell(const std::string& spell_name, int spell_damage, int spell_range, int spell_cost)
    : BaseSpell(spell_name, "Places a trap that deals " + std::to_string(spell_damage) + " damage when enemy steps on it", spell_range, spell_cost, false), base_damage(spell_damage), current_damage(spell_damage) {}


bool TrapSpell::cast(Player& player, const Playing_field& field, int target_x, int target_y, std::vector<Enemy>& enemies) {
    std::vector<std::pair<int, int>> free_cells;
    int player_x = player.get_position_x();
    int player_y = player.get_position_y();
    
    for (int x = std::max(0, player_x - range); x <= std::min(field.get_width() - 1, player_x + range); x++) {
        for (int y = std::max(0, player_y - range); y <= std::min(field.get_length() - 1, player_y + range); y++) {
            if (field.get_cell(x, y).can_pass()) {
                bool cell_occupied = false;
                
                if (x == player_x && y == player_y) {
                    cell_occupied = true;
                }
                
                for (const auto& enemy : enemies) {
                    if (enemy.get_alive() && enemy.get_position_x() == x && enemy.get_position_y() == y) {
                        cell_occupied = true;
                        break;
                    }
                }
                
                if (has_trap(x, y)) {
                    cell_occupied = true;
                }
                
                if (!cell_occupied) {
                    free_cells.push_back(std::make_pair(x, y));
                }
            }
        }
    }
    
    if (free_cells.empty()) {
        std::cout << "No free cells available for trap placement!" << std::endl;
        return false;
    }
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, free_cells.size() - 1);
    int random_index = dis(gen);
    
    int trap_x = free_cells[random_index].first;
    int trap_y = free_cells[random_index].second;
    
    active_traps.emplace_back(trap_x, trap_y);
    return true;
}

void TrapSpell::check_traps(std::vector<Enemy>& enemies, Player& player) {
    for (auto it = active_traps.begin(); it != active_traps.end(); ) {
        int trap_x = it->first;
        int trap_y = it->second;
        bool triggered = false;
        
        for (auto& enemy : enemies) {
            if (enemy.get_alive() && enemy.get_position_x() == trap_x && enemy.get_position_y() == trap_y) {
                bool was_alive = enemy.get_alive();
                
                enemy.taking_damage(current_damage);
                std::cout << name << " triggered! Enemy at (" << trap_x << ", " << trap_y << ") takes " << current_damage << " damage!" << std::endl;
                
                if (was_alive && !enemy.get_alive()) {
                    player.getting_points(enemy, 3);
                    std::cout << "Trap killed enemy! Player gains 3 points!" << std::endl;
                }
                
                triggered = true;
                break;
            }
        }
        
        if (triggered) {
            it = active_traps.erase(it);
        } else {
            ++it;
        }
    }
}

bool TrapSpell::has_trap(int x, int y) const {
    return std::find(active_traps.begin(), active_traps.end(), std::make_pair(x, y)) != active_traps.end();
}

void TrapSpell::remove_trap(int x, int y) {
    active_traps.erase(std::remove(active_traps.begin(), active_traps.end(), std::make_pair(x, y)),active_traps.end());
}

void TrapSpell::clear_traps() {
    active_traps.clear();
}

void TrapSpell::apply_enhancement(int enhancement_level) {
    current_damage = base_damage + enhancement_level;
    std::cout << name << " enhanced! Damage increased to " << current_damage << std::endl;
}

std::unique_ptr<Spell> TrapSpell::clone() const {
    auto cloned = std::make_unique<TrapSpell>(name, base_damage, range, cost);
    cloned->current_damage = current_damage;
    cloned->active_traps = active_traps;  
    return cloned;
}
    