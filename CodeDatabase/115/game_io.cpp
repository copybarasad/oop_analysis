#include "game_io.h"
#include "trap_spell.h"
#include <iostream>
#include <iomanip>

void GameIO::displayGameInfo(const Player& player, const std::vector<Enemy>& enemies,const EntityManager& entity_manager, const std::vector<EnemyTower>& towers) {
    std::cout << "=== INFO ===" << std::endl;
    std::cout << "Life: " << player.get_curr_life() << "/" << player.get_max_life();
    std::cout << " | Score: " << player.get_score();
    std::cout << " | Weapon: " << (player.get_melee_mode() ? "Melee" : "Ranged") << std::endl;

    int alive_towers = 0;
    int total_towers = 0;
    for (const auto& tower : towers) {
        total_towers++;
        if (tower.is_alive()) {
            alive_towers++;
        }
    }
    int alive_enemies = 0;
    for (const auto& enemy : enemies) {
        if (enemy.get_alive()) alive_enemies++;
    }
    
    std::cout << "Enemies: " << alive_enemies << "/" << enemies.size() << std::endl;
}

void GameIO::displayField(const Playing_field& field, const Player& player, const std::vector<Enemy>& enemies, const EntityManager& entity_manager,const std::vector<EnemyTower>& towers) {
    std::cout << "=== GAME FIELD ===" << std::endl;

    auto active_traps = entity_manager.get_active_trap_positions(); 
    
    for (int y = 0; y < field.get_length(); ++y) {
        for (int x = 0; x < field.get_width(); ++x) {
            const auto& cell = field.get_cell(x, y);
            char display_char = '.';
            
            if (!cell.can_pass()) {
                display_char = '#';  
            } else if (x == player.get_position_x() && y == player.get_position_y()) {
                display_char = 'P';  
            } else {
                bool trap_here = false;
                for (const auto& trap_pos : active_traps) {  
                    if (trap_pos.first == x && trap_pos.second == y) {
                        display_char = '!';
                        trap_here = true;
                        break;
                    }
                }
                
                if (!trap_here) {
                    bool ally_here = false;
                    for (const auto& ally : entity_manager.get_allies()) {
                        if (ally.is_alive() && ally.get_position_x() == x && ally.get_position_y() == y) {
                            display_char = 'A';  
                            ally_here = true;
                            break;
                        }
                    }
                    
                    if (!ally_here) {
                        bool enemy_here = false;
                        for (const auto& enemy : enemies) {
                            if (enemy.get_alive() && enemy.get_position_x() == x && enemy.get_position_y() == y) {
                                display_char = 'E';  
                                enemy_here = true;
                                break;
                            }
                        }
                        
                        if (!enemy_here) {
                            for (const auto& tower : towers) {
                                if (tower.is_alive() && tower.get_position_x() == x && tower.get_position_y() == y) {
                                    display_char = 'T';
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            
            std::cout << display_char << ' ';
        }
        std::cout << std::endl;
    }
    
    std::cout << "P = Player, E = Enemy, A = Ally, T = Tower, ! = Trap, # = Wall, . = Empty" << std::endl;
    std::cout << std::endl;
}

void GameIO::displayMessage(const std::string& message) {
    std::cout << ">> " << message << std::endl;
}

void GameIO::displaySpellInfo(const Player& player) {
    std::cout << "=== SPELL INFORMATION ===" << std::endl;
    std::cout << "Enhancement Stacks: " << player.get_enhancement_stacks() << std::endl;
    std::cout << "Spell Slots: " << player.get_spell_hand().get_spell_count() << "/" << player.get_spell_hand().get_max_size() << std::endl;
    
    if (player.get_enhancement_stacks() > 0) {
        std::cout << "Next spell will be enhanced!" << std::endl;
    }
    
    player.get_spell_hand().display_spells();
    std::cout << "=========================" << std::endl;
}

void GameIO::displayControls() {
    std::cout << "=== CONTROLS ===" << std::endl;
    std::cout << "Movement: w (up), a (left), s (down), d (right)" << std::endl;
    std::cout << "Combat: f (attack), r (switch weapon)" << std::endl;
    std::cout << "Spells: c (cast spell), b (spell store)" << std::endl;
    std::cout << "System: z (save), l (load), q (quit)" << std::endl;
    std::cout << "=================" << std::endl;
}