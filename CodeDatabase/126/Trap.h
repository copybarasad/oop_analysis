#ifndef TRAP_H
#define TRAP_H

#include <memory>
#include "GameObject.h"
#include "../field/Position.h"

class GameObject;

class Trap : public GameObject {
    Position position;
    int damage;
    bool active;
    
public:
    Trap(const Position& pos, int dmg = 20);
    
    bool isAlive() const override;
    void takeDamage(int) override;
    Position getPosition() const override;
    
    void trigger(std::shared_ptr<GameObject> target);
    
    std::string serialize() const override;
    static std::shared_ptr<Trap> deserialize(const std::string& data);
};

#endif