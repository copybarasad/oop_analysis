#ifndef SPELL_H
#define SPELL_H

#include <vector>
#include <string>
#include "player.h"
#include "enemy.h"
#include "GameTypes.h"

class ISpell {
public:
    virtual ~ISpell() {};

    virtual SpellResult UseSpell(std::vector<Enemy>& enemies, int playerX, int playerY, 
                               const std::vector<std::pair<int,int>>& enemyPositions,
                               int targetX, int targetY) = 0;
    
    virtual std::string GetName() const = 0;

};

#endif