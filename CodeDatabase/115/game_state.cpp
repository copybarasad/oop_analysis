#include "game_state.h"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream> 

GameState::GameState(int width, int height, bool add_initial_spell)
    : is_running(true), field(width, height), player(5, 5), entity_manager(field, player, enemies) {
    
    if (add_initial_spell) {
        player.get_spell_hand().add_random_spell(entity_manager);
    }
    
    spawn_towers(1);  
    
    static bool seeded = false;
    if (!seeded) {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        seeded = true;
    }
}

void GameState::spawn_towers(int count) {
    towers.clear();
    
    for (int i = 0; i < count; ++i) {
        int x, y;
        int attempts = 0;
        
        do {
            x = std::rand() % (field.get_width() - 4) + 2;
            y = std::rand() % (field.get_length() - 4) + 2;
            attempts++;
            
            if (attempts > 50) {
                if (i == 0) { x = 2; y = 2; }
                else if (i == 1) { x = field.get_width() - 3; y = field.get_length() - 3; }
                else if (i == 2) { x = 2; y = field.get_length() - 3; }
                else { x = field.get_width() - 3; y = 2; }
                break;
            }
        } while (!field.get_cell(x, y).can_pass() || (x == player.get_position_x() && y == player.get_position_y()) ||is_near_player(x, y));
        
        towers.emplace_back(x, y);
    }
}

bool GameState::is_near_player(int x, int y) const {
    int dist_x = std::abs(x - player.get_position_x());
    int dist_y = std::abs(y - player.get_position_y());
    return dist_x <= 3 && dist_y <= 3;
}

bool GameState::isRunning() const {
    return is_running;
}

Playing_field& GameState::getField() {
    return field;
}

Player& GameState::getPlayer() {
    return player;
}

std::vector<Enemy>& GameState::getEnemies() {
    return enemies;
}

EntityManager& GameState::getEntityManager() {
    return entity_manager;
}

std::vector<EnemyTower>& GameState::getTowers() {
    return towers;
}

const Playing_field& GameState::getField() const {
    return field;
}

const Player& GameState::getPlayer() const {
    return player;
}

const std::vector<Enemy>& GameState::getEnemies() const {
    return enemies;
}

const EntityManager& GameState::getEntityManager() const {
    return entity_manager;
}

const std::vector<EnemyTower>& GameState::getTowers() const {
    return towers;
}

void GameState::stopGame() {
    is_running = false;
}

