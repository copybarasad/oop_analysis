#pragma once
#include "character.h"

class Enemy : public Character {
private:
    int damagePower;

public:
    Enemy(std::string name, Field& field, int startX, int startY);
    ~Enemy() = default;
    bool moveTo(int dx, int dy, Player& player, std::string& message);
    void setDamage(int newDamage);
    void setName(std::string newname);
    int getDamage() const;
};