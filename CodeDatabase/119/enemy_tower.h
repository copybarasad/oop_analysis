#ifndef EMENYTOWER_H
#define EMENYTOWER_H
#include "entity.h"
#include "single_target.h"

class EnemyTower : public Entity{

private:
    int radius;
    int attackCooldown;
    int maxCooldown;
    int dmg;
    SingleTarget lowSpell;

public:
    EnemyTower(int radius = 3, int cooldown = 3, int dmg = 8, int towerBaseHp = 50) 
    : Entity(towerBaseHp, 0, 0, "EnemyTower", 'T', EVIL, dmg, 10),
    radius(radius),
    attackCooldown(0),
    maxCooldown(cooldown),
    dmg(dmg) {
        lowSpell = SingleTarget(dmg, radius);
    }
    void update();
    void tryAttack(int targetX, int targetY, Entity* gamer);
    bool canAttack() const {return attackCooldown == 0; }
    int getRadius() const {return radius;}
};


#endif
