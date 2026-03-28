#include "movement.h"


Movement::Movement(Player& player_, Place& place_)
:player(player_), place(place_){}

int Movement::delta_x(Direction direct){
    switch (direct){
        case Direction::Left: return -1;
        case Direction::Right: return 1;
        default: return 0;
    }
}

int Movement::delta_y(Direction direct){
    switch (direct){
        case Direction::Dawn: return -1;
        case Direction::Up: return 1;
        default: return 0;
    }
}


int Movement::move(Direction direct){
    int res_x = player.get_x() + delta_x(direct);
    int res_y = player.get_y() + delta_y(direct);
    Cell::Type res_place = place.get_cell(res_x, res_y);
    if (res_place == Cell::Type::Empty){
        if (place.get_cell(player.get_x(), player.get_y()) == Cell::Type::Player_slow){
            place.change_cell(Cell::Type::Slow, player.get_x(), player.get_y());
        } else if (place.get_cell(player.get_x(), player.get_y()) == Cell::Type::Player_trap){
            place.change_cell(Cell::Type::Trap, player.get_x(), player.get_y());
        } else {
            place.change_cell(Cell::Type::Empty, player.get_x(), player.get_y());
        }
        place.change_cell(Cell::Type::Player, res_x, res_y);
        player.move(res_x, res_y);
        return 1;
    } else if (res_place == Cell::Type::Slow){
        place.change_cell(Cell::Type::Empty, player.get_x(), player.get_y());
        place.change_cell(Cell::Type::Player_slow, res_x, res_y);
        player.move(res_x, res_y);
        return 2;
    } else if (res_place == Cell::Type::Trap){
        place.change_cell(Cell::Type::Empty, player.get_x(), player.get_y());
        place.change_cell(Cell::Type::Player_trap, res_x, res_y);
        player.move(res_x, res_y);
        return 1;
    } else{
        std::cout << "Некорректное действие, пожалуйста повторите" << '\n';
        return 0;
    }
}



Movement::~Movement(){};