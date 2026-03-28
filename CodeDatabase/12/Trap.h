#ifndef TRAP_H
#define TRAP_H

#include "Spell.h"
#include <string>

class Trap : public Spell {
private:
    int Damage;

public:
    Trap(int damage = 20);
    void cast(Player& player, std::vector<Enemy>& enemies, Field& field) override;
    std::string getName() const override;
};

#endif