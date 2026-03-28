#include "ally.h"
#include "enemy.h"
#include "playing_field.h"
#include "player.h" 
#include "event_manager.h"
#include <cmath>
#include <iostream>
#include <random>

Ally::Ally(int x, int y, int unit_health, int unit_damage)
    : position_x(x), position_y(y), health(unit_health), damage(unit_damage), alive(true) {}

void Ally::move_independently(const Playing_field& field, std::vector<Enemy>& enemies, Player& player) {
    if (!alive) return;

    Enemy* closest_enemy = nullptr;
    int min_distance = 1000;
    for (auto& enemy : enemies) {
        if (enemy.get_alive()) {
            int dist_x = std::abs(position_x - enemy.get_position_x());
            int dist_y = std::abs(position_y - enemy.get_position_y());
            int distance = dist_x + dist_y;
            if (distance < min_distance) {
                min_distance = distance;
                closest_enemy = &enemy;
            }
        }
    }

    if (closest_enemy && min_distance <= 1) {
        attack_enemy(*closest_enemy, player); 
        return;
    }

    if (closest_enemy) {
        int target_x = closest_enemy->get_position_x();
        int target_y = closest_enemy->get_position_y();
        int new_x = position_x;
        int new_y = position_y;
        
        if (std::abs(target_x - position_x) > std::abs(target_y - position_y)) {
            if (target_x > position_x) new_x++;
            else if (target_x < position_x) new_x--;
        } else {
            if (target_y > position_y) new_y++;
            else if (target_y < position_y) new_y--;
        }

        if (new_x >= 0 && new_x < field.get_width() &&
            new_y >= 0 && new_y < field.get_length() &&
            field.get_cell(new_x, new_y).can_pass()) {
            position_x = new_x;
            position_y = new_y;
        }
    }
}

void Ally::attack_enemy(Enemy& enemy, Player& player) {
    if (alive && enemy.get_alive()) {
        EventManager::trigger(EventType::DamageDealt, "Ally", damage);
        std::cout << "Ally attacks enemy for " << damage << " damage!" << std::endl;
        
        bool was_alive = enemy.get_alive();
        
        enemy.taking_damage(damage);
        
        if (was_alive && !enemy.get_alive()) {
            player.getting_points(enemy, 3); 
            std::cout << "Ally defeated an enemy! Player gains 3 points!" << std::endl;
        }
    }
}

void Ally::take_damage(int damage_amount) {
    health -= damage_amount;
    EventManager::trigger(EventType::DamageTaken, "Ally", damage_amount, health);
    if (health <= 0) {
        health = 0;
        alive = false;
        std::cout << "Ally defeated!" << std::endl;
        EventManager::trigger(EventType::UnitDied, "Ally defeated");
    }
}

int Ally::get_position_x() const { return position_x; }
int Ally::get_position_y() const { return position_y; }
bool Ally::is_alive() const { return alive; }