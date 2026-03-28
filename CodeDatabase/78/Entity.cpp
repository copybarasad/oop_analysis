#include "GameField.h"

Entity::Entity(int health, int x, int y, WeaponType weapon)
    : health_(health), max_health_(health), position_x_(x), position_y_(y),
      weapon_(weapon) {}

void Entity::set_position(int x, int y) {
    position_x_ = x;
    position_y_ = y;
}

void Entity::set_health(int health) {
    if (health < 0) health = 0;
    if (health > max_health_) health = max_health_;
    health_ = health;
}

void Entity::take_damage(int damage) {
    health_ -= damage;
    if (health_ < 0) health_ = 0;
}

void Entity::heal(int amount) {
    health_ += amount;
    if (health_ > max_health_) {
        health_ = max_health_;
    }
}

bool Entity::can_attack(GameField& game_field, int dx, int dy){

    double distance = sqrt(dx*dx + dy*dy);
    if(distance > sqrt(pow(weapon_.get_range(), 2) + 1)){
        return false;
    }

    int target_x = get_x() + dx;
    int target_y = get_y() + dy;

    if(!game_field.is_coordinates_valid(target_x, target_y)){
        return false;
    }

    return is_line_of_sight_clear(game_field, get_x(), get_y(),
            target_x, target_y);
}

bool Entity::is_line_of_sight_clear(GameField& game_field,
        int start_x, int start_y, int end_x, int end_y) const {

    int dx = abs(end_x - start_x);
    int dy = abs(end_y - start_y);
    int sx = (start_x < end_x) ? 1 : -1;
    int sy = (start_y < end_y) ? 1 : -1;
    int err = dx - dy;
    
    int x = start_x;
    int y = start_y;
    
    while (true) {
        if ((x != start_x || y != start_y) && (x != end_x || y != end_y)) {
            if (!game_field.is_cell_passable(x, y)) {
                return false;
            }
        }
        
        if (x == end_x && y == end_y) break;
        
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x += sx;
        }
        if (e2 < dx) {
            err += dx;
            y += sy;
        }
    }
    
    return true;
}

void Entity::show_stats() const {
    std::cout << "Health: " << health_ << "/" << max_health_
              << "\nPosition: (" << position_x_ << ", " << position_y_ << ")" 
              << "\nWeapon: " << weapon_.get_name()
              << " (Damage: " << weapon_.get_damage() 
              << ", Range: " << weapon_.get_range() << ")" << std::endl;
}

std::string Entity::serialize() const {
    std::string data;

    data += "HEALTH " + std::to_string(health_) + "\n";
    data += "MAX_HEALTH " + std::to_string(max_health_) + "\n";
    data += "WEAPON " + weapon_.get_name() + "\n";
    data += "POSITION " + std::to_string(position_x_) + " " + std::to_string(position_y_) + "\n";

    return data;
}