#include <QTimer>
#include <ctime>
#include <stdlib.h>
#include <algorithm>
#include <array>
#include "enemy.h"



Enemy::Enemy(int x, int y)  {
    health = 100;
    damage = 5;
    enemy_position = nullptr;
    f = 0;
    aliveEnemy = true;
    count_for_slow = 0;
    spawn_enemy(x, y);
}

Enemy::~Enemy(){
    delete enemy_position;
}

void Enemy::subtract_health(int value)
{
    this->health -= value;
    if(health < 0){
        this->health = 0;
        aliveEnemy = false;
    }
}

void Enemy::set_alive(bool v)
{
    this->aliveEnemy = v;
}

void Enemy::set_damage(int v)
{
    damage = v;
}

int Enemy::get_health() const
{
    return this->health;
}

void Enemy::spawn_enemy(int x, int y){
    this->enemy_position = new Point(x, y, unlock);
}


int Enemy::get_coord_x() const
{
    return this->enemy_position->get_point_x();
}

int Enemy::get_coord_y() const
{
    return this->enemy_position->get_point_y();
}

bool Enemy::get_alive() const
{
    return aliveEnemy;
}

int Enemy::get_count_for_slow()
{
    return count_for_slow;
}

int Enemy::get_max_health()
{
    return 100;
}

int Enemy::get_damage()
{
    return damage;
}

void Enemy::recovery_health()
{
    this->health = 100;
}

void Enemy::change_health(int n)
{
    health = n;
}




std::vector<int> Enemy::random_journey()
{

    std::vector<int> result_info = {0, 0, 0};

    std::array<std::pair<int, int>, 4> dirs {{ {1,0}, {-1, 0}, {0, 1}, {0, -1}}};
    std::random_shuffle(dirs.begin(), dirs.end());

    if(std::rand() % 100 < 15) return result_info;

    for (auto [dx, dy] : dirs){

        result_info[0] = dx;
        result_info[1] = dy;
        result_info[2] = count_for_slow;
        return result_info;
    }
    return result_info;
}


std::vector<int> Enemy::enemy_atack()
{
    int x = enemy_position->get_point_x();
    int y = enemy_position->get_point_y();

    std::vector<int> info_attack;
    info_attack.push_back(x);
    info_attack.push_back(y);
    info_attack.push_back(damage);

    return info_attack;
}

void Enemy::set_position(int dx, int dy){
    this->enemy_position->set_point_x(dx);
    this->enemy_position->set_point_y(dy);
}

void Enemy::set_count_for_slow(int val)
{
    this->count_for_slow += val;
}

void Enemy::serialize(std::ostream &os) const{
    os.write(reinterpret_cast<const char*> (&health), sizeof(health));
    os.write(reinterpret_cast<const char*> (&damage), sizeof(damage));
    os.write(reinterpret_cast<const char*> (&f), sizeof(f));
    os.write(reinterpret_cast<const char*> (&aliveEnemy), sizeof(aliveEnemy));
    os.write(reinterpret_cast<const char*> (&count_for_slow), sizeof(count_for_slow));
    enemy_position->serialize(os);
}

void Enemy::deserialize(std::istream &is)
{
    is.read(reinterpret_cast<char*> (&health), sizeof(health));
    is.read(reinterpret_cast<char*> (&damage), sizeof(damage));
    is.read(reinterpret_cast<char*> (&f), sizeof(f));
    is.read(reinterpret_cast<char*> (&aliveEnemy), sizeof(aliveEnemy));
    is.read(reinterpret_cast<char*> (&count_for_slow), sizeof(count_for_slow));
    enemy_position->deserialize(is);
}
