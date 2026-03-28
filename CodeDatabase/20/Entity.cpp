#include "../include/Entity.h"
#include "../include/Gamefield.h"
#include <iostream>


void Entity::move(const std::string& direction, Gamefield* gf){

    int new_x = x_pos;
    int new_y = y_pos;

    if (direction == "up") {
        new_y--;
    } else if (direction == "down") {
        new_y++;
    } else if (direction == "left") {
        new_x--;
    } else if (direction == "right") {
        new_x++;
    } else {
        std::cout << "Unknown direction: " << direction << "\n";
        return;
    }

  
    if (new_x < 0 || new_x >= gf->get_leng() || new_y < 0 || new_y >= gf->get_width()) {
        std::cout << "Move out of bounds! (" << new_x << ", " << new_y << ") is outside the gamefield.\n";
        return;
    }

    Cell* target = gf->getCell(new_x, new_y);

  
    if (!target->is_passable()) {
        std::cout << "Cannot move to (" << new_x << ", " << new_y << "): cell is not passable.\n";
        return;
    }

    if (target->is_occupied()) {
        std::cout << "Cannot move to (" << new_x << ", " << new_y << "): cell is already occupied.\n";
        return;
    }

    gf->getCell(x_pos, y_pos)->set_occupied(false);
    x_pos = new_x;
    y_pos = new_y;
    target->set_occupied(true);
    target->set_occupied_by(this);
    syncSprite();

    std::cout << "Entity moved to (" << x_pos << ", " << y_pos << ").\n";
}

void Entity::get_damaged(int dmg){
        hp -= dmg;
        if (hp <= 0) {
            hp = 0;
            onDeath();
        }
    }
    


void Entity::syncSprite() {
    sprite.setPosition(x_pos * CELL_SIZE, y_pos * CELL_SIZE);
}

