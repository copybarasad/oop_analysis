#include "Field.h"

Field::Field() {}
Field::Field(int height, int width) : height(height), width(width) {
    cells.resize(height);
    for(int i = 0; i < height; i++){  
        cells[i].resize(width);
    }
    cells[0][0].set_player(true);

    std::vector<Point> impassible_cells;
    for(int i = 0; i < height * width / 15; i++){
        int start = 0;
        int end_x = width - 1;
        int end_y = height - 1;

        int pos_x = rand() % (end_x - start + 1) + start;
        int pos_y = rand() % (end_y - start + 1) + start;

        impassible_cells.push_back(Point(pos_x, pos_y));
    }

    for(Point pos : impassible_cells){
        cells[pos.x][pos.y].set_impassible(true);
    }

    cells[height/2][width/2].set_enemy_tower(true);
}
Field::~Field() {}


void Field::move_player(Point source, Point destination){
    cells[source.x][source.y].set_player(false);
    cells[destination.x][destination.y].set_player(true);
}

bool Field::move_enemy(Point source, Point destination){
    if(cells[destination.x][destination.y].is_player()){
        cells[source.x][source.y].set_enemy(false);
        return true; // враг достиг игрока
    }
    cells[source.x][source.y].set_enemy(false);
    cells[destination.x][destination.y].set_enemy(true);
    return false;
}
    

void Field::remove_enemy(Point pos) {
    cells[pos.x][pos.y].set_enemy(false);
}
void Field::set_enemy_at(Point pos) {
    cells[pos.x][pos.y].set_enemy(true);
}
void Field::set_trap_at(Point pos, bool value){
    cells[pos.x][pos.y].set_trap(value);
}


bool Field::is_trap_at(Point pos){
    return cells[pos.x][pos.y].is_trap();
}
bool Field::is_impassible_at(Point pos){
    return cells[pos.x][pos.y].is_impassible();
}
bool Field::has_enemy_at(Point pos) {
    return cells[pos.x][pos.y].is_enemy();
}


bool Field::in_the_field(Point pos){
    if(pos.x >=0 && pos.y >=0){
        if(pos.x < width && pos.y < height){
            return true;
        }
    }
    return false;
}
bool Field::can_shift_left(Point pos){
    Point dest(pos.x - 1, pos.y);
    if(in_the_field(dest)){
        if(!is_impassible_at(dest)){
            return true;
        }
    }
    return false;
}
bool Field::can_shift_down(Point pos){
    Point dest(pos.x, pos.y + 1);
    if(in_the_field(dest)){
        if(!is_impassible_at(dest)){
            return true;
        }
    }
    return false;
}
bool Field::can_shift_up(Point pos){
    Point dest(pos.x, pos.y - 1);
    if(in_the_field(dest)){
        if(!is_impassible_at(dest)){
            return true;
        }
    }
    return false;
}
bool Field::can_shift_right(Point pos){
    Point dest(pos.x + 1, pos.y);
    if(in_the_field(dest)){
        if(!is_impassible_at(dest)){
            return true;
        }
    }
    return false;
}


Point Field::get_sizes(){
    Point P(width, height);
    return P;
}
Cell Field::get_cell(int i, int j){ //нужно для графики
    return cells[i][j];
}


Field::Field(const Field& copy){
    cells = copy.cells;
    height = copy.height;
    width = copy.width;
}
Field::Field(Field&& moved){
    cells = move(moved.cells);
    height = moved.height;
    width = moved.width;
}
Field& Field::operator=(Field&& moved){
    if (&moved != this){
        cells = move(moved.cells);
        height = moved.height;
        width = moved.width;

        moved.height = 0;
        moved.width = 0;
    }
    return *this;
}
Field& Field::operator=(const Field& copy){
    if(&copy != this){
        cells = copy.cells;
        height = copy.height;
        width = copy.width;
    }
    return *this;
}


Saving::FieldData Field::serialize() const{
    Saving::FieldData data;
    std::vector<std::vector<Saving::CellData>> result;
    result.resize(height);
    for(int i = 0; i < height; i++){  
        result[i].resize(width);
    }

    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            result[i][j] = cells[i][j].serialize();
        }
    }

    data.cells = result;
    data.height = height;
    data.width = width;

    return data;
}
void Field::deserialize(const Saving::FieldData& data){
    for(int i = 0; i < data.height; i++){
        for(int j = 0; j < data.width; j++){
            cells[i][j].deserialize(data.cells[i][j]);        
        }
    }
    height = data.height;
    width = data.width;
}
