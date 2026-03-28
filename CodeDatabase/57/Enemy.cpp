#include <SFML/Graphics.hpp>
#include "Enemy.hpp"
#include <random>
#include <iostream>

Enemy::Enemy(std::string puth_enemy_png, int startX, int startY, int offsetX, int offsetY, int atack,
     int helpth, std::string name, int level) : Entity(puth_enemy_png, startX, startY, offsetX, offsetY, atack, helpth, name, enumes, level) {}

void Enemy::random_move(Field& field, InterfaceGameLogger& logger_consol) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 3);
    
    int dx = 0, dy = 0;
    switch(dist(gen)) {
        case 0: dx = -1; break;
        case 1: dx = 1; break;
        case 2: dy = -1; break;
        case 3: dy = 1; break;
    }
    this->move(dx, dy, field, logger_consol);
} 

void Enemy::move_towards_target(Field& field, InterfaceGameLogger& logger_consol) {
    Entity* target = find_first_entity_by_status(field, friends);

    if (target) {
        sf::Vector2i direction = find_target_direction(*target);
        move(direction.x, direction.y, field, logger_consol);
    } else {
        random_move(field, logger_consol);
    }
}
