#include "game_logic.h"
#include "spell_store.h"
#include "trap_spell.h"
#include <iostream>

bool GameLogic::isGameOver(const Player& player, const std::vector<Enemy>& enemies) {
    if (!player.get_alive()) return true;
    
    bool allEnemiesDead = true;
    for (const auto& enemy : enemies) {
        if (enemy.get_alive()) {
            allEnemiesDead = false;
            break;
        }
    }
    
    return allEnemiesDead;
}

void GameLogic::processPlayerAction(GameState& gameState, char input) {
    Player& player = gameState.getPlayer();
    std::vector<Enemy>& enemies = gameState.getEnemies();
    std::vector<EnemyTower>& towers = gameState.getTowers();
    const Playing_field& field = gameState.getField();
    
    switch (input) {
        case 'w': case 'a': case 's': case 'd':
            handlePlayerMovement(player, input, field);
            break;
        case 'f':
            handlePlayerAttack(player, enemies, gameState.getTowers()); 
            break;
        case 'r':
            player.switch_weapon();
            std::cout << "Switched to " << (player.get_melee_mode() ? "melee" : "ranged") << " weapon" << std::endl;
            break;
        case 'c':  
            handleSpellCast(gameState);
            break;
        case 'b':  
            handleSpellStore(gameState);
            break;
        default:
            std::cout << "Unknown command: " << input << std::endl;
            break;
    }
}

void GameLogic::handlePlayerMovement(Player& player, char input, const Playing_field& field) {
    int dx = 0, dy = 0;
    switch (input) {
        case 'w': dy = -1; break;
        case 's': dy = 1; break;
        case 'a': dx = -1; break;
        case 'd': dx = 1; break;
    }
    player.move(dx, dy, field);
}

void GameLogic::handlePlayerAttack(Player& player, std::vector<Enemy>& enemies, std::vector<EnemyTower>& towers) {
    bool hit_any_target = false;

    for (auto& enemy : enemies) {
        if (enemy.get_alive()) {
            player.deals_damage(enemy);
            if (!enemy.get_alive()) {
                player.getting_points(enemy);
                hit_any_target = true;
                std::cout << "Enemy defeated! +3 points" << std::endl;
            } else {
                hit_any_target = true;
            }
        }
    }

    for (auto& tower : towers) {
        if (tower.is_alive()) { 
            int distance_x = std::abs(player.get_position_x() - tower.get_position_x());
            int distance_y = std::abs(player.get_position_y() - tower.get_position_y());
            
            bool in_range = player.get_melee_mode() ? (distance_x <= 1 && distance_y <= 1) : (distance_x <= 3 && distance_y <= 3);
            
            if (in_range) {
                int damage = player.get_melee_mode() ? player.get_melee_damage() : player.get_ranged_damage();
                std::cout << "Player attacks tower for " << damage << " damage!" << std::endl;
                tower.take_damage(damage);
                hit_any_target = true;
                
                if (!tower.is_alive()) {
                   Enemy tower_reward(-1, -1);
                    tower_reward.taking_damage(999);
                    
                    player.getting_points(tower_reward, 10); 
                    
                    std::cout << "Tower destroyed! +10 points " << std::endl;
                }
            }
        }
    }

    if (!hit_any_target) {
        std::cout << "Missed" << std::endl;
    }
}

void GameLogic::handleSpellCast(GameState& gameState) {
    Player& player = gameState.getPlayer();
    const Playing_field& field = gameState.getField();
    
    if (player.get_spell_hand().is_empty()) {
        std::cout << "No spells in hand!" << std::endl;
        return;
    }
    
    player.get_spell_hand().display_spells();
    
    std::cout << "Select spell (1-" << player.get_spell_hand().get_spell_count() << ") or 0 to cancel: ";
    int spell_choice;
    std::cin >> spell_choice;
    
    if (spell_choice == 0) return;
    
    if (spell_choice < 1 || spell_choice > player.get_spell_hand().get_spell_count()) {
        std::cout << "Invalid spell selection!" << std::endl;
        return;
    }
    
    Spell* spell = player.get_spell_hand().get_spell(spell_choice - 1);
    if (!spell) {
        std::cout << "Spell not found!" << std::endl;
        return;
    }
    
    int target_x = player.get_position_x();
    int target_y = player.get_position_y();
    
    if (spell->requires_target()) {
        std::cout << "Enter target coordinates (x y): ";
        std::cin >> target_x >> target_y;
    }
    
    bool success = player.cast_spell(spell_choice - 1, gameState.getField(), target_x, target_y, gameState.getEnemies(), gameState.getEntityManager());
    
    if (success) {
        std::cout << "Spell cast successfully!" << std::endl;
    }else{
        std::cout << "Spell failed!" << std::endl;
    }
}

void GameLogic::handleSpellStore(GameState& gameState) {
    SpellStore store(gameState.getEntityManager());
    Player& player = gameState.getPlayer();
    
    int choice;
    do {
        store.display_available_spells(player);
        std::cout << "Select spell to purchase (0 to exit): ";
        std::cin >> choice;
        
        if (choice > 0 && choice <= store.get_spell_count()) {
            if (player.get_spell_hand().is_full()) {
                std::cout << "Hand is full! Cannot purchase new spell." << std::endl;
            } else {
                if (store.purchase_spell(choice - 1, player)) {
                    std::cout << "Spell purchased successfully!" << std::endl;
                } else {
                    std::cout << "Failed to purchase spell!" << std::endl;
                }
            }
        } else if (choice != 0) {
            std::cout << "Invalid selection!" << std::endl;
        }
    } while (choice != 0);
}

void GameLogic::processEnemyActions(std::vector<Enemy>& enemies, Player& player, const Playing_field& field) {
    for (auto& enemy : enemies) {
        if (enemy.get_alive()) {
            enemy.move(player.get_position_x(), player.get_position_y(), player, field);
        }
    }
}

void GameLogic::processTowerActions(GameState& gameState) {
    for (auto& tower : gameState.getTowers()) {
        tower.update(gameState.getPlayer(), gameState.getField());
    }
}

void GameLogic::processAllyActions(GameState& gameState) {
    gameState.getEntityManager().update_allies(gameState.getField(), gameState.getEnemies());
}

void GameLogic::processTrapActions(GameState& gameState) {
    gameState.getEntityManager().update_traps();  
}