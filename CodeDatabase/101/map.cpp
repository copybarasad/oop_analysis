#include "map.h"

Map::Map(const Tower& enemyTower, SpawnBuilding& bd, int x, int y) {
    height = x;
    width = y;
    map_arr = new Point**[height];

    int build_x = bd.get_coord_x();
    int build_y = bd.get_coord_y();

    int left_x = enemyTower.get_left_x();
    int down_y = enemyTower.get_down_y();

    for (int i = 0; i < height; i++) {
        this->map_arr[i] = new Point*[width];
        for (int j = 0; j < width; j++) {
            if(i == build_x && j == build_y){
                this->map_arr[i][j] = new Point(i, j, build);
            }
            else if ( i % 4 == 0 && j % 4  == 0){
                this->map_arr[i][j] = new Point(i, j, lock);
            }
            else if(j % 3 == 0 && i % 5 == 0){
                this->map_arr[i][j] = new Point(i, j, slow);
            }
            else {
                this->map_arr[i][j] = new Point(i, j, unlock);
            }
        }
    }

    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            if(i == left_x && j == down_y){
                for(int cx = 0; cx < 3; cx++) {
                    for(int cy = 0; cy < 3; cy++) {
                        int new_i = i + cx;
                        int new_j = j + cy;
                        if(new_i < height && new_j < width) {
                            this->map_arr[new_i][new_j]->set_option(tower);
                        }
                    }
                }
                i += 2;
                break;
            }
        }
    }
}

Map::Map(const Map& other)
{
    height = other.height;
    width = other.width;

    if (other.map_arr != nullptr){
        map_arr = new Point**[height];
        for (int i = 0; i < height; ++i) {
            map_arr[i] = new Point*[width];
            for (int j = 0; j < width; ++j) {
                map_arr[i][j] = new Point(*other.map_arr[i][j]);
            }
        }
    } else {
        map_arr = nullptr;
    }
}

Map::Map(Map&& other) noexcept
{
    height = other.height;
    width = other.width;
    map_arr = other.map_arr;
    other.map_arr = nullptr;
    other.height = 0;
    other.width = 0;
}

Map& Map::operator=(const Map& other)
{
    if (this != &other) {

        if (map_arr != nullptr) {
            for (int i = 0; i < height; ++i) {
                for (int j = 0; j < width; ++j) {
                    delete map_arr[i][j];
                }
                delete[] map_arr[i];
            }
            delete[] map_arr;
        }

        height = other.height;
        width = other.width;

        if (other.map_arr != nullptr) {
            map_arr = new Point**[height];
            for (int i = 0; i < height; ++i) {
                map_arr[i] = new Point*[width];
                for (int j = 0; j < width; ++j) {
                    map_arr[i][j] = new Point(*other.map_arr[i][j]);
                }
            }
        } else {
            map_arr = nullptr;
        }
    }
    return *this;
}

// Оператор присваивания перемещением
Map& Map::operator=(Map&& other) noexcept
{
    if (this != &other) {
        for (int i = 0; i < height; ++i) {
            delete[] map_arr[i];
        }
        delete[] map_arr;

        height = other.height;
        width = other.width;
        map_arr = other.map_arr;

        other.map_arr = nullptr;
        other.height = 0;
        other.width = 0;
    }
    return *this;
}

Map::~Map()
{
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            delete map_arr[i][j];
        }
        delete[] map_arr[i];
    }
    delete[] map_arr;
}

const Point& Map::get_point(int x, int y) const
{
    return *map_arr[x][y];
}

Point &Map::set_point(int x, int y)
{
    return *map_arr[x][y];
}

int Map::get_height() const{
    return height;
}

int Map::get_width() const{
    return width;
}

void Map::serialize(std::ostream& os) const  {
    os.write(reinterpret_cast<const char*>(&height), sizeof(height));
    os.write(reinterpret_cast<const char*>(&width), sizeof(width));

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            map_arr[i][j]->serialize(os);
        }
    }
    if (os.fail()) {
        throw std::runtime_error("Ошибка записи Map в поток");
    }
}

void Map::deserialize(std::istream& is) {

    is.read(reinterpret_cast<char*>(&height), sizeof(height));
    is.read(reinterpret_cast<char*>(&width), sizeof(width));

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            map_arr[i][j] = new Point();
            map_arr[i][j]->deserialize(is);
        }
    }
    if (is.fail()) {
        throw std::runtime_error("Ошибка чтения Map из потока");
    }
}
