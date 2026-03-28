#include "tower.h"



Tower::Tower(int x, int y) {
    height = 3;
    width = 3;
    left_x = x;
    down_y = y;
    health = 500;
    alive = true;
    tower_position = new Point**[height];
    allocateTower();
}

Tower::~Tower()
{
    deallocateTower();
}


Point*** Tower::get_tower_position() const
{
    return tower_position;
}

int Tower::get_left_x() const
{
    return left_x;
}

int Tower::get_down_y() const
{
    return down_y;
}

int Tower::get_health() const
{
    return health;
}

bool Tower::get_alive() const
{
    return alive;
}

int Tower::get_max_health()
{
    return 500;
}

void Tower::set_alive(bool f)
{
    this->alive = f;
}

void Tower::sub_health(int damage)
{
    this->health -= damage;

    if(this->health <= 0){
        this->health = 0;
        this->alive = false;
        return;
    }
}

void Tower::change_health(int n)
{
    health = n;
}

void Tower::recovery_health()
{
    health = 500;
}

std::vector<int> Tower::attack() const
{
    std::vector<int> info_attack;
    info_attack.push_back(left_x);
    info_attack.push_back(down_y);
    return info_attack;
}

void Tower::serialize(std::ostream& os) const  {
    os.write(reinterpret_cast<const char*>(&height), sizeof(height));
    os.write(reinterpret_cast<const char*>(&width), sizeof(width));
    os.write(reinterpret_cast<const char*>(&left_x), sizeof(left_x));
    os.write(reinterpret_cast<const char*>(&down_y), sizeof(down_y));
    os.write(reinterpret_cast<const char*>(&health), sizeof(health));
    os.write(reinterpret_cast<const char*>(&alive), sizeof(alive));

    if (tower_position && height > 0 && width > 0) {
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                if (tower_position[i] && tower_position[i][j]) {
                    tower_position[i][j]->serialize(os);
                }
            }
        }
    }

    if (os.fail()) {
        throw std::runtime_error("Ошибка записи Tower в поток");
    }
}

void Tower::deserialize(std::istream& is)  {
    is.read(reinterpret_cast<char*>(&height), sizeof(height));
    is.read(reinterpret_cast<char*>(&width), sizeof(width));
    is.read(reinterpret_cast<char*>(&left_x), sizeof(left_x));
    is.read(reinterpret_cast<char*>(&down_y), sizeof(down_y));
    is.read(reinterpret_cast<char*>(&health), sizeof(health));
    is.read(reinterpret_cast<char*>(&alive), sizeof(alive));

    deallocateTower();
    allocateTower();

    if (tower_position && height > 0 && width > 0) {
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                if (tower_position[i] && tower_position[i][j]) {
                    tower_position[i][j]->deserialize(is);
                }
            }
        }
    }

    if (is.fail()) {
        throw std::runtime_error("Ошибка чтения Tower из потока");
    }
}


void Tower::allocateTower() {
    tower_position = new Point**[height];
    for (int i = 0; i < height; ++i) {
        tower_position[i] = new Point*[width];
        for (int j = 0; j < width; ++j) {
            int pointX = left_x + j;
            int pointY = down_y + i;
            tower_position[i][j] = new Point(pointX, pointY, State::tower);
        }
    }
}

void Tower::deallocateTower() {
    if (!tower_position) return;

    for (int i = 0; i < height; ++i) {
        if (tower_position[i]) {
            for (int j = 0; j < width; ++j) {
                delete tower_position[i][j];
            }
            delete[] tower_position[i];
        }
    }
    delete[] tower_position;
    tower_position = nullptr;
}
