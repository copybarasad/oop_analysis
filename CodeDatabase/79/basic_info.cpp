#include "basic_info.h"
BasicInfo::BasicInfo(int maxHealth_, int health_,  int damage_, int x_, int y_, int points_)
: maxHealth(maxHealth_), health(health_), damage(damage_), x(x_), y(y_), points(points_)
{
    if (maxHealth_ <= 0) throw std::invalid_argument("Максимальное здоовье должно быть положительным");
    if (health < 0) health = 0;
    if (health > maxHealth) health = maxHealth;
    if (damage < 1) damage = 1;
    if (points < 0) points = 0;

}

int BasicInfo::get_maxHealth() const{
    return maxHealth;
}

int BasicInfo::get_health() const{
    return health;
}

int BasicInfo::get_x() const{
    return x;
}
int BasicInfo::get_y() const{
    return y;
}

int BasicInfo::get_damage() const{
    return damage;
}

bool BasicInfo::check_health(){
    return (health <= 0);
}


void BasicInfo::damage_recieved(int dam){
    health -= dam;
    if (check_health()){
        health = 0;
    }
}

void BasicInfo::print_current_stats(){
    std::cout << "Макс здоровье: " << maxHealth << "   Текущее здоровье: " << health << '\n';
    std::cout << "Текущий урон: " << damage << "   Координаты: " << x << ' ' << y << '\n' << '\n';
}

int BasicInfo::get_points(){
    return points;
}
