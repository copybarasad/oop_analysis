#include "area_damage_spell.h"
#include "player.h"
#include "playing_field.h"
#include "enemy.h"
#include <cmath>
#include <iostream>

AreaDamageSpell::AreaDamageSpell(const std::string& spell_name, int spell_damage, int spell_range, int area_size, int spell_cost)
    : BaseSpell(spell_name, "Deals " + std::to_string(spell_damage) + " damage in " + std::to_string(area_size) + "x" + std::to_string(area_size) + " area", spell_range, spell_cost, true), base_area_size(area_size), current_damage(spell_damage), current_area_size(area_size) {}

bool AreaDamageSpell::cast(Player& player, const Playing_field& field, int target_x, int target_y, std::vector<Enemy>& enemies) {
    int distance_x = std::abs(player.get_position_x() - target_x);
    int distance_y = std::abs(player.get_position_y() - target_y);
    
    if (distance_x > get_range() || distance_y > get_range()) {
        std::cout << "Target out of range!" << std::endl;
        return false;
    }

    int enemies_hit = 0;
    int enemies_killed = 0;
    
    int start_x = std::max(0, target_x - current_area_size / 2);
    int start_y = std::max(0, target_y - current_area_size / 2);
    int end_x = std::min(field.get_width() - 1, target_x + current_area_size / 2);
    int end_y = std::min(field.get_length() - 1, target_y + current_area_size / 2);

    for (int x = start_x; x <= end_x; x++) {
        for (int y = start_y; y <= end_y; y++) {
            for (auto& enemy : enemies) {
                if (enemy.get_alive() && enemy.get_position_x() == x && enemy.get_position_y() == y) {
                    bool was_alive = enemy.get_alive();
                    
                    enemy.taking_damage(current_damage);
                    enemies_hit++;
                    
                    if (was_alive && !enemy.get_alive()) {
                        enemies_killed++;
                    }
                }
            }
        }
    }

    std::cout << name << " hits " << enemies_hit << " enemies in " << current_area_size << "x" << current_area_size << " area!" << std::endl;
    
    if (enemies_killed > 0) {
        for (int i = 0; i < enemies_killed; i++) {
            player.getting_points(enemies[0], 3); 
        }
        std::cout << enemies_killed << " enemies defeated! +" << (enemies_killed * 3) << " points" << std::endl;
    }

    return enemies_hit > 0;
}

void AreaDamageSpell::apply_enhancement(int enhancement_level) {
    current_area_size = base_area_size + enhancement_level;
    std::cout << name << " enhanced! Area size: " << current_area_size << "x" << current_area_size << std::endl;
}

std::unique_ptr<Spell> AreaDamageSpell::clone() const {
    return std::make_unique<AreaDamageSpell>(*this);
}