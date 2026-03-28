#include "Entity.hpp"
#include "./Field/Cell.hpp"
#include "Hero.hpp"
#include <iostream>
#include <typeinfo>
#include <sstream> 
#include <random>
#include "./Field/TrapCell.hpp"

Entity::Entity(std::string puth_png, int startX, int startY, int offsetX, int offsetY,
     int atack, int helpth, std::string name, EntityStatus type_of_creature, int level) : location{startX, startY}, level(level), offsetX(offsetX), offsetY(offsetY),
      atack(atack), helpth(helpth), max_helpth(helpth), name(name) {
    texture.loadFromFile(puth_png);
    sprite.setTexture(texture);
    sprite.setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f);
    updatePosition();
    flag_low = false;
    this->type_of_creature = type_of_creature;
}

void Entity::updatePosition() {
    sprite.setPosition(offsetX + location.x * CELL_SIZE + CELL_SIZE / 2.f,
         offsetY + location.y * CELL_SIZE + CELL_SIZE / 2.f);
}

void Entity::attacking_someone(Entity& who, Entity& someone, InterfaceGameLogger& logger_consol) {
    std::ostringstream oss;

    someone.helpth -= who.atack;
    std::cout << "-----------------------------------------------------" << std::endl;
    oss << "-----------------------------------------------------" << std::endl;
    logger_consol.log(oss.str());
    oss.str("");

    std::cout << who.name  << " attacks "  << someone.name << " and deals " << who.atack << " damage." << std::endl;
    oss << who.name  << " attacks "  << someone.name << " and deals " << who.atack << " damage." << std::endl;
    logger_consol.log(oss.str());
    oss.str("");

    std::cout << someone.name << " has " << someone.helpth << " health remaining." << std::endl;
    oss << someone.name << " has " << someone.helpth << " health remaining." << std::endl;
    logger_consol.log(oss.str());
    oss.str("");
    
    std::cout << "-----------------------------------------------------" << std::endl;
    oss << "-----------------------------------------------------" << std::endl;
    logger_consol.log(oss.str());
}

void Entity::move(int dx, int dy, Field& field, InterfaceGameLogger& logger_consol) {
    std::vector<std::vector<Cell*>>& cells = field.get_cells();
    std::ostringstream oss;

    if(flag_low) {
        flag_low = false;
        return;
    } 

    int newX = location.x + dx;
    int newY = location.y + dy;


    if (newX < 0 || newY < 0 || newX >= (int)cells.size() || newY >= (int)cells[0].size())
        return;


    if(cells[newX][newY]->Cell_get_use()) {
        Entity* target = cells[newX][newY]->Cell_get_entity();

        if (this->get_status() == friends && target->get_status() == enumes) {
            attacking_someone(*this, *target, logger_consol);
        }
        else if (this->get_status() == enumes && target->get_status() == friends) {
            attacking_someone(*this, *target, logger_consol);
        }
        else {
            return;
        }

        if (target->get_helpth() <= 0) {
            cells[newX][newY]->Cell_send_use(false, nullptr);
            std::cout << target->name << " died!!!" << std::endl;
            oss << target->name << " died!!!" << std::endl;
            logger_consol.log(oss.str());
        }
        return;
    } else if(cells[newX][newY]->Cell_get_use()) return;
    
    if (cells[newX][newY]->get_status_cell() == impassable) {
        return;
    } else if(cells[newX][newY]->get_status_cell() == slow) {
        flag_low = true;
    } else if(cells[newX][newY]->get_status_cell() == trap) {
        TrapCell* trap = static_cast<TrapCell*>(cells[newX][newY]);
        change_helpth(trap->get_damage(), logger_consol, cells);
        oss << this->name << " damaged by a trap for " + std::to_string(trap->get_damage()) + " health " << std::endl;
        field.ramoveTrap(newX, newY);
        logger_consol.log(oss.str());

    }

    cells[location.x][location.y]->Cell_send_use(false);
    location.x = newX;
    location.y = newY;
    updatePosition();
    cells[location.x][location.y]->Cell_send_use(true, this);
    
}

sf::Sprite& Entity::getSprite() { 
    return sprite;
}

int Entity::get_helpth() {
    return helpth;
}

void Entity::change_helpth(int damage, InterfaceGameLogger& logger_consol, std::vector<std::vector<Cell*>>& cells) {
    std::ostringstream oss;

    
    this->helpth -= damage;
    if(this->helpth <= 0) {
        std::cout << this->name << " died!!!" << std::endl;
        oss << this->name << " died!!!" << std::endl;
        logger_consol.log(oss.str());
        cells[location.x][location.y]->Cell_send_use(false, nullptr);

    }
}



Entity* Entity::find_first_entity_by_status(Field& field, EntityStatus target_status) {
    auto grid_size = field.get_grid_size();
    for (unsigned int x = 0; x < grid_size.x; ++x) {
        for (unsigned int y = 0; y < grid_size.y; ++y) {
            auto cell = field.get_cells()[x][y];
            if (cell->Cell_get_use() && cell->Cell_get_entity()->get_status() == target_status) {
                return cell->Cell_get_entity();
            }
        }
    }
    return nullptr;
}

sf::Vector2i Entity::find_target_direction(Entity& target) {
    sf::Vector2i current_pos = this->get_location();
    sf::Vector2i target_pos = target.get_location();
    
    int dx = 0, dy = 0;
    
    if (target_pos.x > current_pos.x) dx = 1;
    else if (target_pos.x < current_pos.x) dx = -1;
    
    if (target_pos.y > current_pos.y) dy = 1;
    else if (target_pos.y < current_pos.y) dy = -1;
    
    if (dx != 0 && dy != 0) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(0, 1);
        
        if (dist(gen) == 0) {
            dy = 0;
        } else {
            dx = 0;
        }
    }
    
    return sf::Vector2i(dx, dy);
}

void Entity::updateOffsets(int newOffsetX, int newOffsetY) {
    offsetX = newOffsetX;
    offsetY = newOffsetY;
    updatePosition();
}

void Entity::setLevel(int lvl, bool load) {
    std::cout << "update_parametrs level for " << name 
              << "old level: " << level  
              << "new level: " << lvl << std::endl;
    level = lvl;

    this->update_parametrs(load);
}

void Entity::update_parametrs(bool load) {
    if(load) {
        int new_helpth = this->max_helpth * this->level;
        int new_damage = this->atack  * this->level;
        std::cout << "update_parametrs called for " << name 
                << " level: " << level 
                << " current health: " << helpth 
                << " new health: " << new_helpth << std::endl;
        this->helpth = new_helpth;
        this->max_helpth = new_helpth;
        this->atack = new_damage;
        if(name == "Hero") {
            Hero* hero = static_cast<Hero*>(this);
            hero->update_damage_rang();
        }
        return;
    }

    if(level == 1) {
        return;
    }
    int new_helpth = (this->max_helpth / (this->level - 1)) * this->level;
    int new_damage = (this->atack / (this->level - 1)) * this->level;
    std::cout << "update_parametrs called for " << name 
              << " level: " << level 
              << " current health: " << helpth 
              << " current max health: " << helpth
              << " new health: " << new_helpth << std::endl;
    this->helpth = new_helpth;
    this->max_helpth = new_helpth;
    this->atack = new_damage;
    if(name == "Hero") {
            Hero* hero = static_cast<Hero*>(this);
            hero->update_damage_rang();
        }
}