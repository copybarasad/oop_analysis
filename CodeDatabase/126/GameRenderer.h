#ifndef GAMERENDERER_H
#define GAMERENDERER_H

#include <memory>
#include <vector>
#include "../field/GameField.h"
#include "../objects/Player.h"
#include "../objects/Enemy.h"
#include "../objects/EnemyTower.h"
#include "../objects/Trap.h"
#include "../enums/Enums.h"

class GameRenderer {
public:
    void renderHeader(const GameField& field, const std::shared_ptr<Player>& player, int currentLevel, 
                      const std::vector<std::shared_ptr<Enemy>>& enemies, 
                      const std::vector<std::shared_ptr<EnemyTower>>& towers, 
                      const std::vector<std::shared_ptr<Trap>>& traps);
    void renderField(const GameField& field);
    void renderGameOver(GameStatus status, int currentLevel, int score);
};

#endif