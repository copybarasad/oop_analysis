#include "Chaosbolt.h"
#include "Player.h"
#include "Field.h" 
#include "Enemy.h"
#include "Tower.h"
#include <cmath>
#include <cstdio>
#include <iostream>
#include <typeinfo>

int Chaosbolt::cast(Player *player, std::mt19937& gen) {
    std::uniform_int_distribution<int> distrib_damage(tower_less_damage, tower_greater_damage);
    int damage = distrib_damage(gen);
    player->deal_damage(damage);
    return 1;
}

int Chaosbolt::cast(std::string coords, Field *field, int *counter_of_enemies, int *counter_of_towers, Player *player) {
    int x;
    int y;
    int result = std::sscanf(coords.c_str(), "%d.%d", &x, &y);
    if(result != 2){
        std::cout << "Неверная команда\n";
        return 0;
    }
    if((std::abs(x - player->get_current_cell()->getX()) + std::abs(y - player->get_current_cell()->getY())) > this->range){
        std::cout << "Выбрана точка за пределами дистанции\n";
        return 0;
    }
    if(field->get_cell(x, y) && field->get_cell(x, y)->getCreature()){
        std::uniform_int_distribution<int> distrib_damage(less_damage, greater_damage);
        int damage = distrib_damage(this->gen);
        field->get_cell(x, y)->getCreature()->deal_damage(damage);
        if(field->get_cell(x, y)->getCreature()->getIs_dead()){
            if(typeid(*field->get_cell(x, y)->getCreature()) == typeid(Enemy)){
                field->remove_object_field(x, y);
                (*counter_of_enemies)--;
                player->kill_up();
                }
            else if(typeid(*field->get_cell(x, y)->getCreature()) == typeid(Tower)){
                field->remove_object_field(x, y);
                (*counter_of_towers)--;
            }
        }
        return 1;
    }
    else{
        std::cout << "Выбрана точка за пределами поля или в выбранной точке нет врага\n";
        return 0;
    }
}