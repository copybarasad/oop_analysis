#include "game_field.h"
#include <iostream>

game_field::game_field(int input_width, int input_height): width(input_width), height(input_height)
{
    cells.resize(height);
    for(int i = 0; i < height; i++){
        cells[i].resize(width);
    }
}

game_field::game_field(const game_field& other)
    : width(other.width), height(other.height), cells(other.cells)
{}

game_field::game_field(game_field&& other) noexcept
    : width(other.width), height(other.height), cells(std::move(other.cells))
{
    other.width = 0;
    other.height = 0;
}

game_field::~game_field() = default;

int game_field::get_width(){
    return width;
}

int game_field::get_height(){
    return height;
}

game_cell& game_field::get_cell(int x, int y){
    return cells[x][y];
}

bool game_field::isValidPosition(int x, int y) const {
    return x >= 0 && x < width && y >= 0 && y < height;
}

void game_field::set_player(int x, int y){
    cells[x][y].setPlayer(true);
}

void game_field::set_enemy(int x,int y){
    cells[x][y].setEnemy(true);
}

std::vector<std::vector<game_cell>> game_field::get_cells(){
    return cells;
}

void game_field::print_field(){
    std::cout<<"   ";
    for(int i = 0; i < width; i++){
        if(i+1>9) std::cout<<i<<"  ";

        else std::cout<<i<<"   ";
    }
    std::cout<<std::endl;
    for(int y = 0; y< height; y++){
        if(y<10)std::cout<<y<<" ";
        else std::cout<<y;
        for(int x = 0; x<width;x++){
            if (cells[x][y].isEmpty()){
                std::cout << "[ ] ";
            }
            else if(cells[x][y].hasPlayer()){
                std::cout<< "[P] ";
            }
            else if (cells[x][y].hasEnemy()){
                std::cout<< "[E] ";
            }
            else if (cells[x][y].hasTower()){
                std::cout << "[T] ";
            }
            else if (cells[x][y].hasTrap()){
                std::cout << "[X] ";
            }
            else if (cells[x][y].isEmpty()){
                std::cout << "[" << x << y <<"] ";
            }
        }
        std::cout<< std::endl;
    }
}
