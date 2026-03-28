#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "GameField.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyBuilding.h"
#include "EnemyTower.h"
#include "Trap.h"
#include "Ally.h"
#include <vector>
#include <iomanip>

class RenderSystem {
private:
    const GameField& field;
    const Player& player;
    const std::vector<Enemy>& enemies;
    const std::vector<EnemyBuilding>& buildings;
    const std::vector<Trap>& traps;
    const std::vector<Ally>& allies;
    const std::vector<EnemyTower>& towers;

public:
    RenderSystem(const GameField& field, const Player& player, 
                 const std::vector<Enemy>& enemies, const std::vector<EnemyBuilding>& buildings,
                 const std::vector<Trap>& traps, const std::vector<Ally>& allies,
                 const std::vector<EnemyTower>& towers);
    void renderGame() const;
};
#endif