#include "fireball_spell.h"
#include "player.h"        
#include "enemy_manager.h" 
#include "position.h"      
#include <iostream>
#include <cmath>

void FireballSpell::Cast(Player& player, EnemyManager& enemies, GameField& field) {
    Position center = player.GetPosition();
    int damage = 20;
    int radius = 2;
    int hit_count = 0;

    for (auto& e : enemies.GetEnemies()) {
        if (!e.IsAlive()) continue;
        int dist = std::abs(e.GetPosition().x - center.x) + std::abs(e.GetPosition().y - center.y);
        if (dist <= radius) {
            e.TakeDamage(damage);
            hit_count++;
        }
    }

    if (hit_count > 0) {
        std::cout << "Огненный шар поразил " << hit_count << " врагов по " << damage << " урона!\n";
    } else {
        std::cout << "Огненный шар не попал ни по одному врагу!\n";
    }
}