#include "Renderer.h"
#include <iostream>
#include <vector>
#include <string>

void Renderer::render(const Field& field, const EntityManager& entity_manager, GameState game_state, const Point& cursor) const {
    renderField(field, entity_manager, game_state, cursor);
    renderUI(entity_manager, game_state, cursor);
}

void Renderer::renderField(const Field& field, const EntityManager& entity_manager, GameState game_state, const Point& cursor) const {
    std::vector<std::string> display_grid(field.getHeight(), std::string(field.getWidth(), '.'));

    for (int y = 0; y < field.getHeight(); ++y) {
        for (int x = 0; x < field.getWidth(); ++x) {
            switch (field.getCellType(x, y)) {
                case CellType::kImpassable: display_grid[y][x] = '#'; break;
                case CellType::kSlowing:    display_grid[y][x] = 'S'; break;
                default:                    display_grid[y][x] = '.'; break;
            }
        }
    }

    for (const auto& trap : entity_manager.getTraps()) {
        Point pos = trap->getPosition();
        if (field.isValid(pos.x, pos.y)) display_grid[pos.y][pos.x] = 'T';
    }
    
    for (const auto& building : entity_manager.getBuildings()) {
        if (building->isAlive()){
            Point pos = building->getPosition();
            if (field.isValid(pos.x, pos.y)) display_grid[pos.y][pos.x] = 'B';
        }
    }
    
    for (const auto& tower : entity_manager.getTowers()) {
        if (tower->isAlive()) {
            Point pos = tower->getPosition();
            if (field.isValid(pos.x, pos.y)) display_grid[pos.y][pos.x] = 'W';
        }
    }
    
    for (const auto& ally : entity_manager.getAllies()) {
        if (ally->isAlive()) {
            Point pos = ally->getPosition();
            if (field.isValid(pos.x, pos.y)) display_grid[pos.y][pos.x] = 'A';
        }
    }

    for (const auto& enemy : entity_manager.getEnemies()) {
        if (enemy->isAlive()) {
            Point pos = enemy->getPosition();
            if (field.isValid(pos.x, pos.y)) display_grid[pos.y][pos.x] = 'E';
        }
    }

    Player* player = entity_manager.getPlayer();
    if (player && player->isAlive()) {
        Point pos = player->getPosition();
        if (field.isValid(pos.x, pos.y)) display_grid[pos.y][pos.x] = 'P';
    }
    
    if (game_state == GameState::kCasting) {
        if(field.isValid(cursor.x, cursor.y))
            display_grid[cursor.y][cursor.x] = 'X';
    }

    for (int y = 0; y < field.getHeight(); ++y) {
        std::cout << display_grid[y] << std::endl;
    }
}

void Renderer::renderUI(const EntityManager& entity_manager, GameState game_state, const Point& cursor) const {
    Player* player = entity_manager.getPlayer();
    if (player) {
         std::cout << "Player HP: " << player->getHealth() 
                   << " | Score: " << player->getScore()
                   << " | Stance: " << (player->getStance() == CombatStance::kMelee ? "Melee" : "Ranged")
                   << " (Damage: " << player->getDamage() << ")" << std::endl;
        
        std::cout << "Spells: ";
        for(size_t i = 0; i < player->getHand().getSpellCount(); ++i) {
            std::cout << "[" << i + 1 << ": " << player->getHand().getSpell(i)->getName() << "] ";
        }
        std::cout << std::endl;
    }

    if (game_state == GameState::kCasting) {
        std::cout << "CURSOR at (" << cursor.x << "," << cursor.y << "): ";
        Entity* entity = const_cast<EntityManager&>(entity_manager).getEntityAt(cursor);
        if (entity) {
            std::cout << "Entity HP: " << entity->getHealth();
        } else {
            std::cout << "Empty";
        }
        std::cout << std::endl;
    }
}