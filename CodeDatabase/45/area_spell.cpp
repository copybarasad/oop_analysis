#include "area_spell.hpp"
#include <iostream>
#include <unistd.h>

AreaSpell::AreaSpell() : Spell(SpellType::Area) {}

bool AreaSpell::cast(Map* map) {
    std::cout << "1-left-up, 2-right-up, 3-right-down, 4-left-down\n";
    char key;
    ssize_t bytesRead = read(STDIN_FILENO, &key, 1);
    if (bytesRead > 0) {
        int playerX = map->getPlayerX();
        int playerY = map->getPlayerY();
        int radius = 2;
        int damage = 150;
        
        int startX, endX, startY, endY;
        switch (key) {
            case '1': case '2': case '3': case '4':
                startX = (key == '1' || key == '4') ? playerX - radius + 1 : playerX;
                endX = (key == '2' || key == '3') ? playerX + radius - 1 : playerX;
                startY = (key == '1' || key == '2') ? playerY - radius + 1 : playerY;
                endY = (key == '3' || key == '4') ? playerY + radius - 1 : playerY;
                break;
            default:
                return false;
        }
        
        for (int x = startX; x <= endX; x++) {
            for (int y = startY; y <= endY; y++) {
                if (x >= 0 && x < map->getWidth() && y >= 0 && y < map->getHeight() && !(x == playerX && y == playerY)) {
                    map->applyDamageAt(x, y, damage);
                    //std::cout << "COORDS: x: " << x << ", y: " << y << ", PLAYER COORDS: " << playerX << ", " << playerY << "\n";
                }
            }
        }
        return true;
    }
    return false;
}