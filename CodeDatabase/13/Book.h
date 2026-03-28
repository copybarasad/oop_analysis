#ifndef GAME_BOOK_H
#define GAME_BOOK_H

#include "../basics/Weapon.h"

class Book : public Weapon {
public:
    explicit Book(Entity *owner) : Weapon(owner, "SpellBook", 0, 0) {
    };
};
#endif
