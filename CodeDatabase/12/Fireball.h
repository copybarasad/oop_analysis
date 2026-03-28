#ifndef FIREBALL_H
#define FIREBALL_H

#include "Spell.h"
#include <string>

class Fireball : public Spell {
private:
    int Damage;
    int Radius;

public:
    Fireball(int damage = 25, int radius = 3);
    void cast(Player& player, std::vector<Enemy>& enemies, Field& field) override;
    std::string getName() const override;
};

#endif