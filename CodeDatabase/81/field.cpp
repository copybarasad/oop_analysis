#include "Field.h"

Field::Field() : 
    width(MIN(MAX(WIDTH, 10), 25)), 
    height(MIN(MAX(HEIGHT, 10), 25)),
    info_field(height, std::vector<Cell>(width))
{
    for(int y = 0; y < this->height; y++){
        for(int x = 0; x < this->width; x++){
            info_field[y][x] = Cell(x, y);
        }
    }
}
Field::Field(int w, int h) :
    width(MIN(MAX(w, 10), 25)),
    height(MIN(MAX(h, 10), 25)),
    info_field(height, std::vector<Cell>(width))
{
    for(int y = 0; y < this->height; y++){
        for(int x = 0; x < this->width; x++){
            info_field[y][x] = Cell(x, y);
        }
    }
}

int Field::getWidth() const { return this->width; }
int Field::getHeight() const { return this->height; }

void Field::resize(int w, int h){
    w = MIN(MAX(w, 10), 25);
    h = MIN(MAX(h, 10), 25);
    this->width = w;
    this->height = h;

    info_field.resize(this->height, std::vector<Cell>(this->width));
    for(int y = 0; y < this->height; y++){
        info_field[y].resize(this->width);
        for(int x = 0; x < this->width; x++){
            info_field[y][x] = Cell(x, y);
        }
    }
}

Cell* Field::get_cell(int x, int y) {
    if (x >= 0 && x < this->width && y >= 0 && y < this->height) {
        return &info_field[y][x];
    }
    return nullptr;
}

bool Field::place_object_field(int x, int y, ObjectType obj) {
    Cell* cell = get_cell(x, y);
    if(cell && (cell->getObject() == static_cast<char>(ObjectType::Empty) || cell->getObject() == static_cast<char>(ObjectType::Spike))){
        cell->place_object(obj);
        return true;
    }
    return false;
}

bool Field::place_object_field(int x, int y, ObjectType obj, Creature *creature) {
    Cell* cell = get_cell(x, y);
    if(cell && (cell->getObject() == static_cast<char>(ObjectType::Empty) || cell->getObject() == static_cast<char>(ObjectType::Spike))){
        cell->place_object(obj, creature);
        return true;
    }
    return false;
}

void Field::remove_object_field(int x, int y) {
    Cell* cell = get_cell(x, y);
    if (cell) {
        cell->remove_object();
    }
}

void Field::print_field() const {
    std::cout << "\n  ";
    for (int x = 0; x < this->width; x++) {
        std::cout << x << " ";
    }
    std::cout << "\n+";
    for(int x = 0; x < this->width; x++) std::cout << "--";
    std::cout << "+\n";

    for(int y = 0; y < this->height; y++){
        std::cout << y << "|";
        for(int x = 0; x < this->width; x++){
            std::cout << info_field[y][x].getObject() << " ";
        }
        std::cout << "|\n";
    }
    std::cout << "+";
    for(int x = 0; x < this->width; x++) std::cout << "--";
    std::cout << "+\n";
}

void Field::save(std::ofstream& ofs) const {
    ofs << width << " " << height << "\n";
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            ofs << static_cast<int>(info_field[y][x].getObject()) << " ";
        }
        ofs << "\n";
    }
}

void Field::load(std::ifstream& ifs) {
    int w, h;
    if (!(ifs >> w >> h)) {
        throw std::runtime_error("Field load failed to read dimensions."); 
    }
    this->resize(w, h);
    
    int obj_val;
    for (int y = 0; y < height; y++) { 
        for (int x = 0; x < width; x++) {
            if (!(ifs >> obj_val)) {
                throw std::runtime_error("Field load failed to read cell data."); 
            }
            ObjectType obj = static_cast<ObjectType>(obj_val);
            if (obj == ObjectType::Player || obj == ObjectType::Enemy || obj == ObjectType::Tower) {
                obj = ObjectType::Empty;
            }
            info_field[y][x] = Cell(x, y);
            info_field[y][x].place_object(obj);
        }
    }
}