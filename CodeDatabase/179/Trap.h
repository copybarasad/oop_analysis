#ifndef TRAP_H
#define TRAP_H

#include "GameObject.h"

class Trap : public GameObject {
private:
    int damage;
    bool active;

public:
    Trap(int damage = 25, const std::string& name = "Trap");
    
    int getDamage() const;
    bool isActive() const;
    void trigger();
    
    std::string getType() const override;
};

#endif
