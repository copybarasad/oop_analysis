#include "entity_manager.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iostream>

EntityManager::EntityManager(Playing_field& field, Player& player, std::vector<Enemy>& enemies)
    : field(field), player(player), enemies(enemies) {
    
    static bool seeded = false;
    if (!seeded) {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        seeded = true;
    }
}

void EntityManager::spawnEnemies(int count) {
    for (int i = 0; i < count; ++i) {
        int x, y;
        int attempts = 0;
        do {
            x = std::rand() % (field.get_width() - 2) + 1;
            y = std::rand() % (field.get_length() - 2) + 1;
            attempts++;
            if (attempts > 100) {
                std::cout << "Warning: Could not find empty spot for enemy after 100 attempts" << std::endl;
                break;
            }
        } while (!field.get_cell(x, y).can_pass() || 
                 (x == player.get_position_x() && y == player.get_position_y()));
        
        enemies.emplace_back(x, y);
    }
}

void EntityManager::add_ally(const Ally& ally) {
    allies.push_back(ally);
}

void EntityManager::update_allies(const Playing_field& field, std::vector<Enemy>& enemies) {
    for (auto& ally : allies) {
        if (ally.is_alive()) {
            ally.move_independently(field, enemies, player); // Передаем player
        }
    }
    allies.erase(
        std::remove_if(allies.begin(), allies.end(),
            [](const Ally& ally) { return !ally.is_alive(); }),
        allies.end()
    );
}

void EntityManager::clear_allies() {
    allies.clear();
}

int EntityManager::get_ally_count() const {
    return allies.size();
}

const std::vector<Ally>& EntityManager::get_allies() const {
    return allies;
}

void EntityManager::add_trap_spell(std::unique_ptr<TrapSpell> trap_spell) {
    if (trap_spell) {
        active_trap_spells.push_back(std::move(trap_spell));
    }
}

void EntityManager::check_traps() {
    for (auto& trap_spell : active_trap_spells) {
        trap_spell->check_traps(enemies, player); 
    }
}

std::vector<std::pair<int, int>> EntityManager::get_active_trap_positions() const {
    std::vector<std::pair<int, int>> trap_positions;
    for (const auto& trap_spell : active_trap_spells) {
        const auto& traps = trap_spell->get_active_traps();  
        trap_positions.insert(trap_positions.end(), traps.begin(), traps.end());
    }
    return trap_positions;
}

int EntityManager::get_trap_count() const {
    int count = 0;
    for (const auto& trap_spell : active_trap_spells) {
        count += trap_spell->get_active_traps().size();  
    }
    return count;
}

void EntityManager::update_traps() {
    for (auto& trap_spell : active_trap_spells) {
        trap_spell->check_traps(enemies, player);  
    }
    active_trap_spells.erase(
        std::remove_if(active_trap_spells.begin(), active_trap_spells.end(),
            [](const std::unique_ptr<TrapSpell>& trap) {
                return trap->get_active_traps().empty();
            }),
        active_trap_spells.end()
    );
}

const Player& EntityManager::getPlayer() const {
    return player;
}

const std::vector<Enemy>& EntityManager::getEnemies() const {
    return enemies;
}