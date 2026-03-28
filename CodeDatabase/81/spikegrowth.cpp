#include "Spikegrowth.h"
#include "Player.h"
#include "Field.h" 
#include "constants.h"
#include <cmath>
#include <cstdio>
#include <iostream>
#include <typeinfo>

int Spikegrowth::cast(std::string coords, Field *field, int *counter_of_enemies, int *counter_of_towers, Player *player) {
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
    if(field->get_cell(x, y)->getObject() != static_cast<char>(ObjectType::Empty)){
        std::cout << "Нельзя вырастить шипы на занятой клетке\n";
        return 0;
    }
    field->place_object_field(x, y, ObjectType::Spike);
    return 1;
}