#ifndef AREADAMAGE_H
#define AREADAMAGE_H

#include "spell.h"
#include "GameTypes.h" // это кароче для сохр результатов 

class AreaDamage: public ISpell {
private:
    int damage;
    int radius;
public:
    AreaDamage(int damage, int radius);
    
    SpellResult UseSpell(std::vector<Enemy>& enemies, int playerX, int playerY, const std::vector<std::pair<int,int>>& enemyPositions,int targetX, int targetY) override;
    
    std::string GetName() const override;
    

};

#endif