#include "teleport_spell.h"
#include "player.h"        
#include "enemy_manager.h" 
#include "game_field.h"    
#include "position.h"      
#include <random>
#include <iostream>

void TeleportSpell::Cast(Player& player, EnemyManager& enemies, GameField& field){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> x_dis(0, field.GetWidth() - 1);
    std::uniform_int_distribution<> y_dis(0, field.GetHeight() - 1);

    Position new_pos;
    int attempts = 0;
    do {
        new_pos = {x_dis(gen), y_dis(gen)};
        attempts++;
    } while ((!field.IsWalkable(new_pos) || player.CheckEnemyCollision(enemies, new_pos)) && attempts < 100);

    if (attempts >= 100) {
        std::cout << "Телепортация не удалась — нет свободного места!\n";
        return;
    }

    player.SetPosition(new_pos);
    std::cout << "Вы телепортировались на (" << new_pos.x << "," << new_pos.y << ")!\n";
}