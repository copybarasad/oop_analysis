#pragma once
#include <Entity.hpp>
#include <Constants.hpp>

class PlayerTrap : public Entity {
public:
    PlayerTrap(int x, int y);
    virtual void draw() override;
    int getDamage() const;
    void empower(int charges);
    void setDamage(int value);
    
private:
    char sprite = PLAYER_TRAP_SPRITE;
    int damage;
};