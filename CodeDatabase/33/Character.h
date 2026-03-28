#pragma once
#include "Coord.h"

class Character {
public:
    Character();
    Character(int hp, int damage, Coord pos);

    int HP() const;
    void SetHP(int hp);
    int Damage() const;

    Coord Pos() const;
    void SetPos(Coord p);

    bool IsAlive() const;
    void TakeDamage(int dmg);

protected:
    int hp_;
    int damage_;
    Coord pos_;
};
