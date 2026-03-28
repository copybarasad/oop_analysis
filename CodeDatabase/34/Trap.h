#ifndef GAME_TRAP_H
#define GAME_TRAP_H

#include "../utils/Point.h"
#include <iostream>

namespace Game {

class Trap {
public:
    Trap(const Utils::Point& position, int damage);
    Trap(std::istream& is); // Конструктор для загрузки

    const Utils::Point& getPosition() const;
    int getDamage() const;
    bool isTriggered() const;
    void trigger();

    void serialize(std::ostream& os) const;

private:
    void deserialize(std::istream& is);

    Utils::Point position;
    int damage;
    bool triggered;
};

}

#endif