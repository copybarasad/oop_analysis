#pragma once
#include <vector>
#include <fstream>
#include "player.hpp"
#include "enemy.hpp"
#include "enemy_building.hpp"
#include "field.hpp"
#include "potion.hpp"
#include "trap_enity.hpp"
#include "enemy_tower.hpp"
#include "ally.hpp"

class Game {
private:
    Player player_;
    Field field_;
    std::vector<Enemy> enemies_;
    std::vector<EnemyBuilding> buildings_;
    std::vector<Trap> traps_;
    std::vector<EnemyTower> towers_;
    std::vector<Ally> allies_;
    int level_;

    void InitEnemies();
    void InitPotions();
    void InitBuildings();
    void InitTowers();

    std::pair<int, int> FindFreeCell();
public:
    Game();

    bool IsLevelCompleted() const;
    void NextLevel();             
    void GenerateLevel(int level);
    int GetLevel() const;

    void MoveEnemies();
    void MoveAllies();
    void PrintState();

    void EnemyTryMeleeAttack(Enemy& e, int playerX, int playerY);
    void EnemyTryAttackAlly(Enemy& e);
    void AllyTryAttackEnemy(Ally& a);

    void AddTrap(int x, int y, int damage);
    void CheckTraps();
    void AddAlly(const Ally& ally);

    Player& GetPlayer() const;
    Field& GetField() const;

    void SaveEntities(std::ostream& out) const;
    void LoadEntities(std::istream& in);
    void SetupEmpty(int w, int h, int level);
    void ForceResizeField(int w, int h);
    void SetLevel(int lvl);
    
    Player& GetPlayer();
    Field& GetField();
    std::vector<Enemy>& GetEnemies();
    std::vector<EnemyBuilding>& GetBuildings();
    std::vector<EnemyTower>& GetTowers();
    std::vector<Trap>& GetTraps();
    std::vector<Ally>& GetAllies();
};
