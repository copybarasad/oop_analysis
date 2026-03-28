#include "magic_tower.h"
#include <iostream>

MagicTower::MagicTower(int castInterval_, int maxHealth_, int health_, int x_, int y_, int _radius, int points_)
: BasicInfo(maxHealth_, health_, 0, x_, y_, points_), castInterval(castInterval_), radius(_radius), castCounter(0), spell()
{
    if (castInterval_ <= 0) castInterval = 3;
}

void MagicTower::update(Place& place, Player& player, std::vector<Enemy> enemies, Tower& tower) {
    if (!check_health()) {
        int pl_x = player.get_x();
        int pl_y = player.get_y();
        if (abs(pl_x - x) <= radius  && abs(pl_y - y) <= radius){
            castCounter++;
            if (castCounter >= castInterval){
                spell.attack(player, 0, 0, place, enemies, tower, *this);
                castCounter = 0;
            }
        }
    }
}

void MagicTower::print_current_stats() {
    std::cout << "Creature: Magic Tower" << '\n';
    std::cout << "Макс здоровье: " << maxHealth << "   Текущее здоровье: " << health << '\n';
    std::cout << "Координаты: " << x << " " << y << std::endl;
    std::cout << "Следующая атака через: " << (castInterval - castCounter) << " ходов\n\n";
}

MagicTower::~MagicTower() {}