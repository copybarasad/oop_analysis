#include "Trap.h"

Trap::Trap(int x, int y, int damage) 
    : x(x), y(y), damage(damage), active(true) {}

void Trap::activate(Character& character) {
    if (active) {
        character.take_damage(damage);
        std::cout << character.getName() << " наступил на ловушку и получил " 
                  << damage << " урона!" << std::endl;
        active = false;
    }
}