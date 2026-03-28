#pragma once
#include "types.hpp"
#include "GameObjects/Entities/interfaces.hpp"


class PersonBaseClass : public Fightable {
public:
    Coordinates getPosition() override;
    void setPosition(Coordinates pos);

    int getHealthPoints() const;
    void minusHealthPoints(int hp) override;
    void setHealthPoints(int hp);

    int getDamagePoints() const;
    void setDamagePoints(int dmg);

    bool isDisabled() const;
    void changeDisabled();
    void setDisabled(bool d);

    int getMaxHP() const;
    void setMaxHP(int hp);

protected:
    Coordinates position;
    int maxHP = 0;
    int healthPoints = 0;
    int damagePoints = 0;
    bool disabled = false;
};