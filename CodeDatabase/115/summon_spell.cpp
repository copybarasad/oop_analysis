#include "summon_spell.h"
#include "player.h"
#include "playing_field.h"
#include "enemy.h"
#include "entity_manager.h"
#include <iostream>

SummonSpell::SummonSpell(const std::string& spell_name, int spell_cost, int spell_range, EntityManager& manager, int summon_count)
    : BaseSpell(spell_name, "Summons " + std::to_string(summon_count) + " ally units that move independently",spell_range, spell_cost, false), base_summon_count(summon_count), current_summon_count(summon_count), entity_manager(manager) {}

bool SummonSpell::cast(Player& player, const Playing_field& field, int target_x, int target_y, std::vector<Enemy>& enemies) {
    int summoned = 0;
    int player_x = player.get_position_x();
    int player_y = player.get_position_y();

    for (int dx = -1; dx <= 1 && summoned < current_summon_count; dx++) {
        for (int dy = -1; dy <= 1 && summoned < current_summon_count; dy++) {
            if (dx == 0 && dy == 0) continue;
            
            int spawn_x = player_x + dx;
            int spawn_y = player_y + dy;
            
            if (spawn_x >= 0 && spawn_x < field.get_width() &&
                spawn_y >= 0 && spawn_y < field.get_length() &&
                field.get_cell(spawn_x, spawn_y).can_pass()) {
                
                bool cell_free = true;
                for (const auto& enemy : enemies) {
                    if (enemy.get_alive() && enemy.get_position_x() == spawn_x && enemy.get_position_y() == spawn_y) {
                        cell_free = false;
                        break;
                    }
                }
                
                if (cell_free) {
                    Ally new_ally(spawn_x, spawn_y, 1, 1);
                    entity_manager.add_ally(new_ally);
                    summoned++;
                }
            }
        }
    }
    
    std::cout << name << " summoned " << summoned << " allies!" << std::endl;
    return summoned > 0;
}


void SummonSpell::apply_enhancement(int enhancement_level) {
    current_summon_count = base_summon_count + enhancement_level;
    std::cout << name << " enhanced! Summon count increased to " << current_summon_count << std::endl;
}

std::unique_ptr<Spell> SummonSpell::clone() const {
    return std::make_unique<SummonSpell>(*this);
}