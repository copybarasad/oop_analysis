#include "place.h"

Place::Place(int rows_, int cols_)
: rows(rows_), cols(cols_)
{
    if (rows < 10) rows = 10;
    if (rows > 25) rows = 25;
    if (cols < 10) cols = 10;
    if (cols > 25) cols = 25;
    field.assign(rows, std::vector<Cell>(cols));
}

int Place::get_rows() const{
    return rows;
}

int Place::get_cols() const{
    return cols;
}

Cell::Type Place::get_cell(int x, int y){
    if ((x >= 0) && (x < get_rows()) && (y >= 0) && (y < get_cols())){
        Cell cell = field.at(x).at(y);
        return cell.get_type();
    }
    return Cell::Type::Obstacle;

}

int Place::check_cell(int x, int y){
    if (get_cell(x, y) != Cell::Type::Obstacle){
        return true;
    } else{
        return false;
    }
}

void Place::change_cell(Cell::Type type, int x, int y){
    if (check_cell(x, y)){
        field.at(x).at(y).change_type(type);
    }
}

void Place::create_splecial_cell(Cell::Type cell_type){
    int way;
    switch (cell_type){
        case Cell::Type::Obstacle:
            way = rows - cols;
            if (way < 0) way = -1;
            if (way > 0) way = 1;
            break;
        case Cell::Type::Slow:
            way = 2;
            break;
        default: std::cout << "Некорректный тип." << '\n'; break;
    }
    int special = 6;
    for(int x = 0; x <= rows; x++){
        for(int y = 0; y <= cols; y++){
            if (x % special == 0 && y % special == 0){
                change_cell(cell_type, x, y);
                switch (way){
                    case 0:
                        change_cell(cell_type, x + 1, y);
                        change_cell(cell_type, x - 1, y);
                        change_cell(cell_type, x, y + 1);
                        change_cell(cell_type, x, y - 1);
                        break;
                    case 1:
                        change_cell(cell_type, x + 1, y);
                        change_cell(cell_type, x - 1, y);
                        break;
                    case -1:
                        change_cell(cell_type, x, y + 1);
                        change_cell(cell_type, x, y - 1);
                        break;
                    case 2:
                        change_cell(cell_type, x + 1, y + 4);
                        change_cell(cell_type, x + 4, y - 1);
                        break;
                }
            }
        }
    }
}

void Place::render(){
    for (int i = 0; i < cols; i++){
        if (i < 9){
            std::cout << i << ' ';
        } else {
            std::cout << i;
        }
    }
    std::cout << '\n';
    for(int x = 0; x < rows; x++){
        for(int y = 0; y < cols; y++){
            char ch = '.';
            switch (get_cell(x, y)){
                case Cell::Type::Empty: ch = '.'; break;
                case Cell::Type::Player: ch = 'P'; break;
                case Cell::Type::Enemy:  ch = 'E'; break;
                case Cell::Type::Tower:  ch = 'T'; break;
                case Cell::Type::Obstacle: ch = '#'; break;
                case Cell::Type::Slow: ch = 'O'; break;
                case Cell::Type::Player_slow: ch = 'P'; break;
                case Cell::Type::Enemy_slow: ch = 'E'; break;
                case Cell::Type::Trap: ch = 'X'; break;
                case Cell::Type::Player_trap: ch = 'P'; break;
                case Cell::Type::Magic_tower: ch = 'M'; break;
            }
            std::cout << ch << ' ';
        }
        std::cout << x << ' ';
        std::cout << '\n';
    }
}

Place::Place(const Place &other)
:rows(other.rows), cols(other.cols), field(other.field){}

Place::Place(Place&& other) noexcept
:rows(other.rows), cols(other.cols), field(std::move(other.field)){
    other.rows = 0;
    other.cols = 0;
}

Place& Place::operator=(const Place& other){
    if (&other != this){
        rows = other.rows;
        cols = other.cols;
        field = other.field;
    }
    return *this;
}

Place& Place::operator=(Place&& other) noexcept{
    if (&other != this){
        rows = other.rows;
        cols = other.cols;
        field = std::move(other.field);
        other.rows = 0;
        other.cols = 0;
    }
    return *this;
}
Place::~Place(){}
