#include "trap_spell.hpp"
#include <iostream>
#include <unistd.h>
#include "map.hpp"
#include "cell.hpp"

TrapSpell::TrapSpell() : Spell(SpellType::Trap) {}

bool TrapSpell::cast(Map* map) {
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

        if (0 <= x && x < map->getWidth() && 0 <= y && y < map->getHeight() && map->getCellState(x, y) == CellState::EMPTY) {
            map->setTrapAt(x, y);
            return true;
        }

        return false;
    }
    return false;
}