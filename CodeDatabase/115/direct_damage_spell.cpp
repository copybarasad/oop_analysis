#include "direct_damage_spell.h"
#include "player.h"
#include "playing_field.h"
#include "enemy.h"
#include <cmath>
#include <iostream>

DirectDamageSpell::DirectDamageSpell(const std::string& spell_name, int spell_damage, int spell_range, int spell_cost)
    : BaseSpell(spell_name, "Deals " + std::to_string(spell_damage) + " damage to single target", spell_range, spell_cost, true), current_damage(spell_damage), current_range(spell_range) {}

bool DirectDamageSpell::cast(Player& player, const Playing_field& field, int target_x, int target_y, std::vector<Enemy>& enemies) {
    int distance_x = std::abs(player.get_position_x() - target_x);
    int distance_y = std::abs(player.get_position_y() - target_y);
    
    if (distance_x > current_range || distance_y > current_range) {
        std::cout << "Target out of range!" << std::endl;
        return false;
    }

    bool hit = false;
    for (auto& enemy : enemies) {
        if (enemy.get_alive() && enemy.get_position_x() == target_x && enemy.get_position_y() == target_y) {
            bool was_alive = enemy.get_alive();
            
            enemy.taking_damage(current_damage);
            std::cout << name << " hits enemy for " << current_damage << " damage!" << std::endl;
            
            if (was_alive && !enemy.get_alive()) {
                player.getting_points(enemy, 3);
                std::cout << "Enemy defeated! +3 points" << std::endl;
            }
            
            hit = true;
            break;
        }
    }

    if (!hit) {
        std::cout << "No valid target at specified location!" << std::endl;
        return false;
    }

    return true;
}

void DirectDamageSpell::apply_enhancement(int enhancement_level) {
    current_range = range + enhancement_level;
    std::cout << name << " enhanced! Range: " << current_range << std::endl;
}

std::unique_ptr<Spell> DirectDamageSpell::clone() const {
    return std::make_unique<DirectDamageSpell>(*this);
}