#pragma once
#include <vector>

#include "Cell.h"
#include "EnemyBuilding.h"
#include "EnemyTower.h"

class Field {
public:
    Field(int width, int height, int num_buildings, int enemy_health, int enemy_damage);
    Field(const Field& other);
    Field(Field&& other) noexcept;

    Field& operator=(const Field& other);
    Field& operator=(Field&& other) noexcept;

    std::vector<EnemyBuilding>& GetBuildings();
    const std::vector<EnemyBuilding>& GetBuildings() const;
    std::vector<EnemyTower>& GetTowers();
    const std::vector<EnemyTower>& GetTowers() const;

    int GetWidth() const;
    int GetHeight() const;
    int GetNumBuildings() const;
    
    void ClearCell(int x, int y);
    bool IsInside(int x, int y) const;
    void Display() const;
    bool IsCellEmpty(int x, int y) const;
    bool AreNeighborsEmpty(int x, int y) const;
    std::vector<std::pair<int,int>> GetAvailableCells() const;
    CellType GetCellType(int x, int y) const;
    void SetCellType(int x, int y, CellType type);

    static int CellTypeToInt(CellType t);
    static CellType IntToCellType(int v);

private:
    int width_;
    int height_;
    int num_buildings_;
    int spawn_enemy_health_;
    int spawn_enemy_damage_;
    std::vector<std::vector<Cell>> grid_;
    std::vector<EnemyBuilding> enemy_buildings_;
    std::vector<EnemyTower> enemy_towers_;
};
