#ifndef WORLD_H
#define WORLD_H

#include "field.h"
#include "player.h"
#include "enemy.h"
#include "enemy_base.h"
#include "enemy_tower.h"
#include <vector>

class World {
public:
    World(int width = 10, int height = 10);
    World(int width, int height, int level);
    World(int width, int height, int level, Player&& player);
    ~World() = default;
    World(const World&) = delete;
    World& operator=(const World&) = delete;
    World(World&& other) noexcept;
    World& operator=(World&& other) noexcept;
    const Field& GetField() const;
    const Player& GetPlayer() const;
    Player& MutablePlayer();
    bool IsGameOver() const;
    bool IsBaseAlive() const;
    int GetTurn() const;
    void SetTurn(int turn);
    void MovePlayer(int dr, int dc);
    void PlayerAttack();
    bool PlayerCastSpell(int idx, const std::pair<int,int>& target);
    void AdvanceTurn();
    void PrintStatus() const;
    void SwitchPlayerMode();
    int GetLevel() const;

    const std::vector<Enemy>& GetEnemies() const { return enemies_; }
    const std::vector<EnemyBase>& GetBases() const { return bases_; }
    const std::vector<EnemyTower>& GetTowers() const { return towers_; }
    void ClearEnemies();
    void ClearBases();
    void ClearTowers();
    void SetEnemies(std::vector<Enemy> enemies);
    void SetBases(std::vector<EnemyBase> bases);
    void SetTowers(std::vector<EnemyTower> towers);

private:
    void TryPlaceEnemyRandomly();
    void ApplyPendingSpellDamages();
    void TowerAttackPhase();
    void UpdateEnemies();
    void UpdateAllies();
    void EnemySpawnPhase();

    Field field_;
    Player player_;
    std::vector<Enemy> enemies_;
    std::vector<EnemyBase> bases_;
    std::vector<EnemyTower> towers_;
    int turn_;
    int level_;
};

#endif