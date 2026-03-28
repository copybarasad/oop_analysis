#include "combat_system.h"
#include <cmath>
#include <iostream>

void CombatSystem::attack(GameObject* attacker, GameObject* target) {
    if (is_range(attacker, target)) {
        int damage = calculate_damage(attacker, target);
        target->takeDamage(damage);
        
        if (target->getHP() <= 0) {
            Player* player = dynamic_cast<Player*>(attacker);
            if (player != nullptr) {
                player->addKill();
            }
        }
    }
}

bool CombatSystem::player_attack(Player* player, Field& field, int dx, int dy) {
    if (Freezy_System::isFrozen(*player)) {
        Freezy_System::freezyReduceObject(*player);
        return true;
    }
    
    int x_player = player->getX();
    int y_player = player->getY();
    int target_x = x_player + dx;
    int target_y = y_player + dy;
    
    if (dx == 0 && dy == 0) {
        return false;
    }
    if (target_x < 0 || target_x >= field.getWidth() || target_y < 0 || target_y >= field.getHeight()) {
        return false;
    }
    
    if (field.getTypePlace(target_x, target_y) < PLACE_ENEMY) {
        return false;
    }
    
    const auto& enemies = field.getEnemies();
    for (size_t i = 0; i < enemies.size(); i++) {
        if (enemies[i]->getX() == target_x && enemies[i]->getY() == target_y) {
            CombatSystem::attack(player, enemies[i].get());
            
            if (enemies[i]->getHP() <= 0) {
                field.setTypePlace(target_x, target_y, PLACE_EMPTY);
                field.removeEnemy(i);
            }
            return true;
        }
    }
    
    int building_count = field.getCntBuilding();
    for (int i = 0; i < building_count; i++) {
        Building* building = field.getBuild(i);
        if (building && building->getX() == target_x && building->getY() == target_y) {
            CombatSystem::attack(player, building);
            
            if (building->getHP() <= 0) {
                field.setTypePlace(target_x, target_y, PLACE_EMPTY);
                field.removeBuilding(i);
            }
            return true;
        }
    }
    int spell_tower_count = field.getCntSpellTower();
    for (int i = 0; i < spell_tower_count; i++) {
        SpellTower* tower = field.getSpellTower(i);
        if (tower && tower->getX() == target_x && tower->getY() == target_y) {
            CombatSystem::attack(player, tower);
            
            if (tower->getHP() <= 0) {
                field.setTypePlace(target_x, target_y, PLACE_EMPTY);
                field.removeSpellTower(i);
            }
            return true;
        }
    }

    return false;
}

bool CombatSystem::is_range(GameObject* attacker, GameObject* target) {
    int x_stroke_length = std::abs(attacker->getX() - target->getX());
    int y_stroke_length = std::abs(attacker->getY() - target->getY());
    
    if (attacker->getTypeCombat() == "melee") {
        return x_stroke_length <= 1 && y_stroke_length <= 1;
    } else {
        return x_stroke_length + y_stroke_length <= attacker->getRange(); 
    }
}

int CombatSystem::calculate_damage(GameObject* attacker, GameObject* target) {
    return attacker->getDamage();
}