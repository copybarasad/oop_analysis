#include "../include/Enemy.h"
#include "Constants.h"
#include <iostream>

Enemy::Enemy(int hp, int dmg, int x, int y) {
    this->hp = hp;
    this->damage = dmg;
    this->x_pos = x;
    this->y_pos = y;

    if (!this->texture.loadFromFile("../sprites/enemy.png")){
        std::cerr<<"file loading error";
    }
    float scaleX = static_cast<float>(CELL_SIZE) / this->texture.getSize().x;
    float scaleY = static_cast<float>(CELL_SIZE) / this->texture.getSize().y;
    
    this->sprite.setTexture(texture);
    this->sprite.setScale(scaleX,scaleY);

    
    
    syncSprite();
}

Enemy::Enemy() {
    this->hp = 50;
    this->damage = 10;
    this->x_pos = 0;
    this->y_pos = 0;
}


void Enemy::move(const std::string& direction, Gamefield* gf) {
    int new_x = x_pos;
    int new_y = y_pos;

    if (direction == "up") new_y--;
    else if (direction == "down") new_y++;
    else if (direction == "left") new_x--;
    else if (direction == "right") new_x++;
    else {
        std::cout << "Unknown direction: " << direction << "\n";
        return;
    }

    if (new_x < 0 || new_x >= gf->get_leng() || new_y < 0 || new_y >= gf->get_width()) {
        std::cout << "Move out of bounds! (" << new_x << ", " << new_y << ")\n";
        return;
    }

    Cell* target = gf->getCell(new_x, new_y);

    if (!target->is_passable()) {
        std::cout << "Cannot move to (" << new_x << ", " << new_y << "): cell not passable.\n";
        return;
    }

    if (target->is_occupied()) {
        Entity* entity = target->is_occupied_by();
        Player* player = dynamic_cast<Player*>(entity);
        if (player) {
            std::cout << "На клетке игрок!\n";
            player->get_damaged(damage);
        }
        return;
    }

    if (target->is_traped()) {
        std::cout << "Enemy was caught in a trap at (" 
                  << target->x_position() << ", " 
                  << target->y_position() << ")!\n";
        this->get_damaged(10);
        target->set_traped(false);
    }

    Cell* current = gf->getCell(x_pos, y_pos);
    current->set_occupied(false);
    current->set_occupied_by(nullptr);

    x_pos = new_x;
    y_pos = new_y;

    target->set_occupied(true);
    target->set_occupied_by(this);

    syncSprite();

    std::cout << "Enemy moved to (" << x_pos << ", " << y_pos << ").\n";
}



Enemy::Enemy(const Enemy& other)
{
    this->hp = other.hp;
    this->damage = other.damage;
    this->x_pos = other.x_pos;
    this->y_pos = other.y_pos;

    if (!this->texture.loadFromFile("../sprites/enemy.png")) {
        std::cerr << "file loading error in copy constructor\n";
    }

    this->sprite.setTexture(this->texture);

    float scaleX = static_cast<float>(CELL_SIZE) / this->texture.getSize().x;
    float scaleY = static_cast<float>(CELL_SIZE) / this->texture.getSize().y;

    this->sprite.setScale(scaleX, scaleY);

    syncSprite();
}

Enemy& Enemy::operator=(const Enemy& other)
{
    if (this == &other)
        return *this;

    this->hp = other.hp;
    this->damage = other.damage;
    this->x_pos = other.x_pos;
    this->y_pos = other.y_pos;

    if (!this->texture.loadFromFile("../sprites/enemy.png")) {
        std::cerr << "file loading error in assignment operator\n";
    }

    this->sprite.setTexture(this->texture);

    float scaleX = static_cast<float>(CELL_SIZE) / this->texture.getSize().x;
    float scaleY = static_cast<float>(CELL_SIZE) / this->texture.getSize().y;

    this->sprite.setScale(scaleX, scaleY);

    syncSprite();

    return *this;
}





