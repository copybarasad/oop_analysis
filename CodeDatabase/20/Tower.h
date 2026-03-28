#include "Enemy.h"
#include "Gamefield.h"
#include "Lightning_bolt.h"
#include <unordered_set>
#include <iostream>

class TeslaTower: public Enemy{
private:
    private:
    int range;
    int damage;
    int attackCooldown = 0;   


public:
    //constructors and destructors
    TeslaTower(int hp, int dmg, int x, int y, int range);
    TeslaTower(const TeslaTower& other);             
    TeslaTower& operator=(const TeslaTower& other);


    //functions
    void move(const std::string& direction, Gamefield* gf) override;
    void attack(Gamefield* gf);
    void cast_spell(Gamefield *gf);
    int getRange() const { return range; }
    void attackLine(int x, int y, int dx, int dy, Gamefield* gf,std::unordered_set<void*>& alreadyHit);




};