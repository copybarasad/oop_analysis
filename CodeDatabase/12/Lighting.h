#ifndef LIGHTING_H
#define LIGHTING_H

#include "Spell.h"
#include <string>

class Lighting : public Spell {
private:
    int Damage;
    int Radius;

public:
    Lighting(int damage = 25, int radius = 2);
    void cast(Player& player, std::vector<Enemy>& enemies, Field& field) override;
    std::string getName() const override;
};

#endif