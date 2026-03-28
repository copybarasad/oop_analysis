#include "direct_spell.hpp"
#include <iostream>
#include <unistd.h>

DirectSpell::DirectSpell() : Spell(SpellType::Direct) {}

bool DirectSpell::cast(Map* map) {
    char key;
    int deltax=0;
    int deltay=0;
    ssize_t bytesRead = read(STDIN_FILENO, &key, 1);
    if (bytesRead > 0) {
        switch (key) {
            case 'w':
            case 'W':
                deltay=-1;
                break;
                
            case 'a':
            case 'A':
                deltax=-1;
                break;
                
            case 's':
            case 'S':
                deltay=1;
                break;
                
            case 'd':
            case 'D':
                deltax=1;
                break;
        }
        int x=map->getPlayerX()+deltax;
        int y=map->getPlayerY()+deltay;

        int r = 3;

        while (0 <= x && x < map->getWidth() && 0 <= y && y < map->getHeight() && r) {
            if (map->getCellState(x, y) == CellState::MONSTER || map->getCellState(x, y) == CellState::WALL || map->getCellState(x, y) == CellState::TOWER) {
                shoot(map, x, y, 100);
                return true;
            }
            x+=deltax;
            y+=deltay;
            r -= 1;
        }
        return false;
    }
    return false;
}

void DirectSpell::shoot(Map* map, int x, int y, int damage) {
    map->applyDamageAt(x, y, damage);
}