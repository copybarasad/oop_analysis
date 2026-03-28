#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include "field.hpp"
#include "ally.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "enemy_building.hpp"
#include "enemytower.hpp"
#include "trap.hpp"

class Game; 

class GameMove {
public:
    explicit GameMove(Game& game);

    bool MovePlayer(int dx, int dy);
    void MoveEnemies();
    void MoveAllies();

private:
    Game& game_; 

    EnemyUnit* GetEnemyById(int id);
    bool HandleRangedAttack(int x, int y, int dx, int dy);
    bool HandleMeleeAttack(int nx, int ny);
    std::vector<std::vector<int>> ComputeDistances(int playerX, int playerY) const;
    bool AttackEnemy(EnemyUnit* e, int targetX, int targetY);
    EnemyUnit* FindClosestEnemy(const Ally& ally);
    void ResolveEnemyCell(EnemyUnit& e, int ex, int ey, int bestX, int bestY, int playerX, int playerY);
    void AllyAttackEnemy(Ally& ally, EnemyUnit& enemy);
    void MoveAllyTowards(Ally& ally, const EnemyUnit& target);
};
