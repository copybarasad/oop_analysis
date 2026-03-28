#ifndef DIRECTGAMAGE_H
#define DIRECTGAMAGE_H

#include "spell.h"

class DirectDamage: public ISpell {
private:
    int damage;
    int radius;
public:
    DirectDamage(int damage, int radius);

    SpellResult UseSpell(std::vector<Enemy>& enemies, int playerX, int playerY, const std::vector<std::pair<int,int>>& enemyPositions,int targetX, int targetY) override;
    
    std::string GetName() const override;

};

#endif