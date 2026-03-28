#include "Fireball.h"
#include "Player.h"
#include "Field.h" 
#include "Enemy.h"
#include "Tower.h"
#include <cmath>
#include <cstdio>
#include <iostream>
#include <typeinfo>

Fireball::Fireball() : Spell(0, "Огненный шар") {}
Fireball::Fireball(int upd) : Spell(upd, "Огненный шар") {}

int Fireball::cast(std::string coords, Field *field, int *counter_of_enemies, int *counter_of_towers, Player *player) {
    int center_x;
    int center_y;
    int result = std::sscanf(coords.c_str(), "%d.%d", &center_x, &center_y);
    
    if(result != 2){
        std::cout << "Неверная команда\n";
        return 0;
    }

    if((std::abs(center_x - player->get_current_cell()->getX()) + std::abs(center_y - player->get_current_cell()->getY())) > this->range){
        std::cout << "Выбрана точка за пределами дистанции\n";
        return 0;
    }

    this->rad += this->upgrades;

    for(int x = (center_x - this->rad); x <= (center_x + this->rad); x++){
        if(x <= center_x){
            for(int y = (center_y - (x - (center_x - this->rad))); y <= (center_y + (x - (center_x - this->rad))); y++){
                if(field->get_cell(x, y) && field->get_cell(x, y)->getCreature()){
                    Creature* target = field->get_cell(x, y)->getCreature();
                    target->deal_damage(this->damage);
                    if(target->getIs_dead()){
                        if(typeid(*target) == typeid(Enemy)){
                            field->remove_object_field(x, y);
                            (*counter_of_enemies)--;
                            player->kill_up();
                        }
                        else if(typeid(*target) == typeid(Tower)){
                            field->remove_object_field(x, y);
                            (*counter_of_towers)--;
                        }
                    }
                }
            }
        }
        else{
            for(int y = (center_y - (center_x + this->rad - x)); y <= (center_y + (center_x + this->rad - x)); y++){
                if(field->get_cell(x, y) && field->get_cell(x, y)->getCreature()){
                    Creature* target = field->get_cell(x, y)->getCreature();
                    target->deal_damage(damage);
                    if(target->getIs_dead()){
                        if(typeid(*target) == typeid(Enemy)){
                            field->remove_object_field(x, y);
                            (*counter_of_enemies)--;
                            player->kill_up();
                        }
                        else if(typeid(*target) == typeid(Tower)){
                            field->remove_object_field(x, y);
                            (*counter_of_towers)--;
                        }
                    }
                }
            }
        }
    }
    return 1;
}