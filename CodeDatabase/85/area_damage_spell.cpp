#include "area_damage_spell.h"
#include "game_controller.h"
#include "enemy.h"
#include <iostream>

AreaDamageSpell::AreaDamageSpell(const std::string& name, const std::string& desc, 
                               int spell_range, int spell_damage, int size)
    : Spell(name, desc, spell_range),
      damage(spell_damage),
      area_size(size) {}

bool AreaDamageSpell::cast(Game_controller& controller, const Position& target) {
    Position player_pos = controller.get_player().get_position();
    int dx = std::abs(player_pos.get_x() - target.get_x());
    int dy = std::abs(player_pos.get_y() - target.get_y());
    
    if (dx > range || dy > range) {
        std::cout << "Target is out of range! Range: " << range << std::endl;
        return false;
    }
    
    std::cout << name << " explodes at position (" << target.get_x() << ", " << target.get_y() << ")!" << std::endl;
    
    int enemies_hit = 0;
    const auto& enemies = controller.get_enemies();
    
    for (int y = target.get_y(); y < target.get_y() + area_size; ++y) {
        for (int x = target.get_x(); x < target.get_x() + area_size; ++x) {
            Position area_pos(x, y);
            
            for (auto& enemy : enemies) {
                if (enemy->get_position() == area_pos) {
                    enemy->take_damage(damage);
                    enemies_hit++;
                    std::cout << "Enemy hit for " << damage << " damage!" << std::endl;
                    
                    if (!enemy->is_alive()) {
                        std::cout << "Enemy defeated!" << std::endl;
                    }
                }
            }
        }
    }
    
    if (enemies_hit == 0) {
        std::cout << "The spell exploded but hit no enemies." << std::endl;
    }
    
    return true;
}