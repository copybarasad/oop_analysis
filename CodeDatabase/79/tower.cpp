#include "tower.h"
#include <iostream>

Tower::Tower(int spawnInterval_, std::vector<Enemy>& enemies_, int maxHealth_, int health_, int x_, int y_, int points_)
: BasicInfo(maxHealth_, health_, 0, x_, y_, points_), spawnCounter(0), spawnInterval(spawnInterval_), enemies(enemies_)
{
    if (spawnInterval_ <= 0) spawnInterval = 3;
}

void Tower::update(Place& place) {
    if (!check_health()) {
        spawnCounter++;
        
        if (spawnCounter >= spawnInterval) {
            int spawnX = -1, spawnY = -1;
            
            int directions[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
            
            for (int i = 0; i < 4; i++) {
                int newX = x + directions[i][0];
                int newY = y + directions[i][1];
                
                if (place.check_cell(newX, newY) && 
                    place.get_cell(newX, newY) == Cell::Type::Empty) {
                    spawnX = newX;
                    spawnY = newY;
                    break;
                }
            }
            
            if (spawnX != -1 && spawnY != -1) {
                Enemy newEnemy(20, 20, 2, spawnX, spawnY);
                enemies.push_back(newEnemy);
                place.change_cell(Cell::Type::Enemy, spawnX, spawnY);
                
                std::cout << "Башня создала врага на координатах: (" << spawnX << ", " << spawnY << ")\n";
                
                spawnCounter = 0; 
            }
        }
    }
}

void Tower::print_current_stats() {
    std::cout << "Creature: Tower" << '\n';
    std::cout << "Макс здоровье: " << maxHealth << "   Текущее здоровье: " << health << '\n';
    std::cout << "Координаты: " << x << " " << y << std::endl;
    std::cout << "Следующий спавн через: " << (spawnInterval - spawnCounter) << " ходов\n\n";
}

Tower::~Tower() {}