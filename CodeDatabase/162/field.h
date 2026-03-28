#ifndef FIELD_H_
#define FIELD_H_

#include "cell.h"
#include "player.h"
#include "enemy.h"
#include "building.h"
#include "trap.h"
#include "enemy_tower.h" 
#include <memory>

class Ally;  

class Field {
public:
    Field(int width, int height, int turns_to_spawn_enemy, int obstacle_count, int slow_count, int building_count, std::shared_ptr<Player> player_param = nullptr);
    Field(const Field& other);
    Field(Field&& other) noexcept;
    Field& operator=(const Field& other);
    Field& operator=(Field&& other) noexcept;
    ~Field();

    void InitializeObstaclesAndSlowCells(int obstacle_count, int slow_count, int building_count);
    void SpawnInitialEnemies(int enemy_count);
    void ProcessBuildings();
    bool IsCellPassable(int x, int y) const;
    bool IsEmpty(int x, int y) const;
    bool MovePlayer(int dx, int dy);
    void MoveEnemies();
    bool AttackEnemiesAt(int x, int y);
    void Print() const;
    void RunGame(bool& save_requested, bool& load_requested, bool& quit_requested);
    bool IsPlayerAlive() const;
    bool IsPlayerSlowed() const;
    void ResetPlayerSlowed();
    void PlayerSwitchMode();
    int GetPlayerX() const;
    int GetPlayerY() const;
    Player* GetPlayer() const;
    Enemy* GetEnemyAt(int x, int y) const;
    Building* GetBuildingAt(int x, int y) const;
    void RemoveEnemy(Enemy* enemy);
    void RemoveBuilding(Building* building);
    void RemoveAlly(Ally* ally);
    void AddTrap(int x, int y, int damage);
    void UpdateTraps();
    void AddTower(int x, int y);
    void UpdateTowers();
    void AddAlly(int x, int y);
    void UpdateAllies();
    std::vector<std::vector<Cell>>& GetGrid() { return grid_; }
    const std::vector<std::vector<Cell>>& GetGrid() const { return grid_; }
    bool IsValidPosition(int x, int y) const { return x >= 0 && x < width_ && y >= 0 && y < height_; }
    const std::vector<std::unique_ptr<Building>>& GetBuildings() const { return buildings_; }
    const std::vector<std::unique_ptr<EnemyTower>>& GetTowers() const { return towers_; }
    EnemyTower* GetTowerAt(int x, int y) const;
    void RemoveTower(EnemyTower* tower);
    const std::vector<std::unique_ptr<Ally>>& GetAllies() const { return allies_; }
    const std::vector<std::unique_ptr<Trap>>& GetTraps() const { return traps_; }
    int GetWidth() const { return width_; }
    int GetHeight() const { return height_; }
    int GetTurnCount() const { return turn_count_; } 
    void SetPlayerX(int x) { player_x_ = x; }
    void SetPlayerY(int y) { player_y_ = y; }
    void SetTurnCount(int c) { turn_count_ = c; }
    void SetPlayerSlowed(bool b) { player_slowed_ = b; }
    void SetPlayer(std::shared_ptr<Player> player, int x, int y) {
        player_ = player;
        player_x_ = x;
        player_y_ = y;
        if (grid_[y][x].GetType() == CellType::kEmpty) {
            grid_[y][x].SetType(CellType::kPlayer);
            under_player_type_ = CellType::kEmpty;
        }
    }

    std::vector<std::unique_ptr<Building>>& GetBuildings() { return buildings_; }
    std::vector<std::unique_ptr<EnemyTower>>& GetTowers() { return towers_; }
    std::vector<std::unique_ptr<Ally>>& GetAllies() { return allies_; }
    std::vector<std::unique_ptr<Trap>>& GetTraps() { return traps_; }

    void SetEnemies(std::vector<std::unique_ptr<Enemy>> enemies) { enemies_ = std::move(enemies); }
    void SetBuildings(std::vector<std::unique_ptr<Building>> buildings) { buildings_ = std::move(buildings); }
    void SetTowers(std::vector<std::unique_ptr<EnemyTower>> towers) { towers_ = std::move(towers); }

    int GetTurnsToSpawnEnemy() const { return turns_to_spawn_enemy_; }

    std::vector<std::unique_ptr<Enemy>>& GetEnemies() { return enemies_; }
    const std::vector<std::unique_ptr<Enemy>>& GetEnemies() const { return enemies_; }

    Ally* GetAllyAt(int x, int y) const;

    void SetEnemiesPositions(const std::vector<std::pair<int, int>>& positions, const std::vector<CellType>& under_types, const std::vector<bool>& slowed) {
        enemy_positions_ = positions;
        under_enemy_type_ = under_types;
        enemy_slowed_ = slowed;
    }

    void SetBuildingPositions(const std::vector<std::pair<int, int>>& positions) {
        building_positions_ = positions;
    }

    void SetTowerPositions(const std::vector<std::pair<int, int>>& positions, const std::vector<CellType>& under_types) {
        tower_positions_ = positions;
        under_tower_type_ = under_types;
    }

    void SetAllyPositions(const std::vector<std::pair<int, int>>& positions, const std::vector<CellType>& under_types) {
        ally_positions_ = positions;
        under_ally_type_ = under_types;
    }

    void SetTrapPositions(const std::vector<std::pair<int, int>>& positions, const std::vector<CellType>& under_types) {
        trap_positions_ = positions;
        under_trap_type_ = under_types;
    }

    const std::vector<std::pair<int, int>>& GetEnemiesPositions() const { return enemy_positions_; }
    const std::vector<std::pair<int, int>>& GetBuildingPositions() const { return building_positions_; }
    const std::vector<std::pair<int, int>>& GetTowerPositions() const { return tower_positions_; }
    const std::vector<std::pair<int, int>>& GetAllyPositions() const { return ally_positions_; }

private:

    int width_;
    int height_;
    int turns_to_spawn_enemy_;
    std::vector<std::vector<Cell>> grid_;
    std::shared_ptr<Player> player_;
    int player_x_;
    int player_y_;
    CellType under_player_type_;
    bool player_slowed_;
    std::vector<std::unique_ptr<Enemy>> enemies_;
    std::vector<std::pair<int, int>> enemy_positions_;
    std::vector<CellType> under_enemy_type_;
    std::vector<bool> enemy_slowed_;
    std::vector<std::unique_ptr<Building>> buildings_;
    std::vector<std::pair<int, int>> building_positions_;
    std::vector<std::unique_ptr<Trap>> traps_;
    std::vector<std::unique_ptr<EnemyTower>> towers_;
    std::vector<std::unique_ptr<Ally>> allies_;
    std::vector<std::pair<int, int>> trap_positions_;
    std::vector<CellType> under_trap_type_;
    std::vector<std::pair<int, int>> tower_positions_;
    std::vector<CellType> under_tower_type_;
    std::vector<std::pair<int, int>> ally_positions_;
    std::vector<CellType> under_ally_type_;
    int turn_count_;
};

#endif // FIELD_H_