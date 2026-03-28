#ifndef HEAL_H
#define HEAL_H

#include "Spell.h"
#include <string>

class Heal : public Spell {
private:
    int HealAmount;

public:
    Heal(int amount = 20);
    void cast(Player& player, std::vector<Enemy>& enemies, Field& field) override;
    std::string getName() const override;
};

#endif