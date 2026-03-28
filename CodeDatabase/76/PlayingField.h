#pragma once
#include "Cell.h"
#include <vector>
#include <string>
#include <utility> 

class EnemyTower;  
class Player;
class Enemy;

using namespace std;

class PlayingField {
private:
    int width;
    int height;
    vector<vector<Cell>> grid;
    vector<EnemyTower> towers;

    void InitializeGrid(); 
    void GenerateBarriers(int barrierCount); 
public:
    PlayingField(int width, int height, int barrierCount);
    PlayingField(PlayingField&& other) noexcept;
    PlayingField(const PlayingField& other);
    PlayingField& operator=(const PlayingField& other);
    PlayingField& operator=(PlayingField&& other) noexcept;

    void ClearAllObstacles();
    vector<Cell*> GetBarriers() const;
    bool AddBarrier(int x, int y); 
    
    void RestoreBarriers(const vector<pair<int, int>>& barrierPositions);
    
    EnemyTower* AddTower(int x, int y);

    EnemyTower* GetNearestTower(int playerX, int playerY);
    
    bool IsValidPosition(int x, int y) const;
    bool IsCellFree(int x, int y) const;
    int GetWidth() const;
    int GetHeight() const;
    const vector<vector<Cell>>& GetGrid() const;
    
    void SetCellSymbol(int x, int y, char symbol, bool occupied);
    void FreeCell(int x, int y);
    void DisplayField(const Player& player, const Enemy& enemy) const;
    bool HasTrapAt(int x, int y) const;

    void AddTower(const EnemyTower& tower);
    vector<EnemyTower>& GetTowers();
    const vector<EnemyTower>& GetTowers() const;
    void UpdateTowers(); 
    void CheckTowerAttacks(Player& player); 
    void DisplayTowers() const; 
    bool IsBarrierAt(int x, int y) const;
    void DamageTowersInRange(int centerX, int centerY, int range, int damage);

    int GetBarrierCount() const;
    vector<pair<int, int>> GetBarrierCoordinates() const;

    bool HasBarrierAt(int x, int y) const;
    bool HasTowerAt(int x, int y) const;


    ~PlayingField() = default;
};