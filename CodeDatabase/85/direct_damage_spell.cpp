#include "direct_damage_spell.h"
#include "game_controller.h"
#include "enemy.h"
#include <iostream>

DirectDamageSpell::DirectDamageSpell(const std::string& name, const std::string& desc, 
                                   int spell_range, int spell_damage)
    : Spell(name, desc, spell_range),
      damage(spell_damage) {}

bool DirectDamageSpell::cast(Game_controller& controller, const Position& target) {
    Position player_pos = controller.get_player().get_position();
    
    int dx = std::abs(player_pos.get_x() - target.get_x());
    int dy = std::abs(player_pos.get_y() - target.get_y());
    
    if (dx > range || dy > range) {
        std::cout << "Target is out of range! Range: " << range << std::endl;
        return false;
    }
    
    const auto& enemies = controller.get_enemies();
    
    for (auto& enemy : enemies) {
        if (enemy->get_position() == target) {
            enemy->take_damage(damage);
            std::cout << name << " hits enemy for " << damage << " damage!" << std::endl;
            
            if (!enemy->is_alive()) {
                std::cout << "Enemy defeated by magic!" << std::endl;
            }
            return true;
        }
    }
    
    std::cout << "No valid target at position (" << target.get_x() << ", " << target.get_y() << ")!" << std::endl;
    return false;
}