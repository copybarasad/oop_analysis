#include "PlayingField.h"
#include "Player.h"    
#include "Enemy.h"     
#include "EnemyTower.h"
#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

PlayingField::PlayingField(int width, int height, int barrierCount)
: width(width), height(height) 
{
    InitializeGrid(); 
    EnemyTower tower(width/2, height/2, 100, 100, 3, 2);
    AddTower(tower);
    if (barrierCount > 0) {
        GenerateBarriers(barrierCount);
    }
}

PlayingField::PlayingField(const PlayingField& other) 
    : width(other.width), height(other.height), grid(other.grid) 
{
}

PlayingField::PlayingField(PlayingField&& other) noexcept  
: width(other.width), height(other.height), grid(move(other.grid))  
{ 
    other.width = 0; 
    other.height = 0; 
}  

PlayingField& PlayingField::operator=(const PlayingField& other) 
{
    if (this != &other) 
    {
        if (other.width <= 0 || other.height <= 0) 
        {
            throw invalid_argument("Cannot assign from PlayingField with invalid dimensions");
        }
        width = other.width;
        height = other.height;
        grid = other.grid;
    }
    return *this;
}

PlayingField& PlayingField::operator=(PlayingField&& other) noexcept 
{
    if (this != &other) 
    {
        width = other.width;
        height = other.height;
        grid = move(other.grid);
        other.width = 0;
        other.height = 0;
    }
    return *this;
}

bool PlayingField::IsValidPosition(int x, int y) const 
{
    return x >= 0 && x < width && y >= 0 && y < height && !grid[y][x].IsWall();
}

void PlayingField::GenerateBarriers(int barrierCount) 
{ 
    srand(time(0));  
     
    int barriersPlaced = 0; 
    int attempts = 0; 

    const int maxAttempts = 1000; 
     
    while (barriersPlaced < barrierCount && attempts < maxAttempts)  
    { 
        int x = 1 + rand() % (width - 2); 
        int y = 1 + rand() % (height - 2); 
 
        if ((x == 1 && y == 1) || (x == width - 2 && y == height - 2))  
        { 
            attempts++; 
            continue;  
        } 

        bool isTowerPosition = false;
        for (const auto& tower : towers) 
        {
            if (x == tower.GetX() && y == tower.GetY()) 
            {
                isTowerPosition = true;
                break;
            }
        }
        if (isTowerPosition) 
        {
            attempts++;
            continue;
        }
         
        if (IsCellFree(x, y) && !grid[y][x].IsWall())  
        { 
            AddBarrier(x, y);
            barriersPlaced++; 
        } 
        
        attempts++; 
    } 
} 

bool PlayingField::IsCellFree(int x, int y) const {
    if (!IsValidPosition(x, y)) return false;
    return !grid[y][x].IsOccupied() && !grid[y][x].IsWall() && 
           grid[y][x].GetSymbol() != 'X'; 
}

int PlayingField::GetWidth() const 
{ 
    if (width <= 0) 
    {
        throw logic_error("PlayingField width is in invalid state");
    }
    return width; 
}

int PlayingField::GetHeight() const 
{ 
    if (height <= 0) 
    {
        throw logic_error("PlayingField height is in invalid state");
    }
    return height; 
}

const vector<vector<Cell>>& PlayingField::GetGrid() const 
{ 
    if (grid.empty()) 
    {
        throw logic_error("PlayingField grid is not initialized");
    }
    return grid; 
}

void PlayingField::SetCellSymbol(int x, int y, char symbol, bool occupied) 
{
    if (!IsValidPosition(x, y)) 
    {
        throw out_of_range("Invalid position for SetCellSymbol: (" + to_string(x) + ", " + to_string(y) + ")");
    }
    grid[y][x].SetSymbol(symbol);
    grid[y][x].SetOccupied(occupied);
}

void PlayingField::FreeCell(int x, int y) 
{
    if (!IsValidPosition(x, y)) 
    {
        throw out_of_range("Invalid position for FreeCell: (" + to_string(x) + ", " + to_string(y) + ")");
    }
    grid[y][x].SetSymbol('.');
    grid[y][x].SetOccupied(false);
}

void PlayingField::DisplayField(const Player& player, const Enemy& enemy) const 
{
    if (grid.empty()) 
    {
        throw logic_error("Cannot display field - grid is not initialized");
    }

    for (int i = 0; i < height; i++) 
    {
        for (int j = 0; j < width; j++) 
        {
            char symbol = grid[i][j].GetSymbol();
            
            bool hasPlayer = (j == player.GetX_player() && i == player.GetY_player());
            bool hasEnemy = enemy.IsAlive_enemy() && (j == enemy.GetX_enemy() && i == enemy.GetY_enemy());
            bool hasTower = false;
            
            for (const auto& tower : towers) {
                if (j == tower.GetX() && i == tower.GetY()) {
                    hasTower = true;
                    break;
                }
            }
            
            if (hasPlayer) {
                cout << "P ";
            } else if (hasEnemy) {
                cout << "E ";
            } else if (hasTower) {
                cout << "W ";
            } else if (symbol == 'X') {
                cout << "X ";
            } else {
                cout << symbol << " ";
            }
        }
        cout << endl;
    }
    
    cout << "P - Player, E - Enemy, X - Barrier, W - Enemy Tower, # - Wall" << endl;
}

void PlayingField::InitializeGrid()  
{ 
    grid.resize(height, vector<Cell>(width)); 
     
    for (int i = 0; i < height; i++)  
    { 
        for (int j = 0; j < width; j++)  
        { 
            if (i == 0 || i == height - 1 || j == 0 || j == width - 1)  
            { 
                grid[i][j] = Cell('#', true, false);
            }  
            else  
            { 
                grid[i][j] = Cell('.', false, false);
            } 
        } 
    } 
}  

bool PlayingField::HasTrapAt(int x, int y) const 
{
    if (!IsValidPosition(x, y)) return false;
    return grid[y][x].GetSymbol() == 'T';
}

vector<Cell*> PlayingField::GetBarriers() const {
    vector<Cell*> barrierPtrs;
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (grid[y][x].GetSymbol() == 'X') {
                barrierPtrs.push_back(const_cast<Cell*>(&grid[y][x]));
            }
        }
    }
    return barrierPtrs;
}

bool PlayingField::AddBarrier(int x, int y) {
    if (!IsValidPosition(x, y)) {
        return false;
    }
    
    if (!IsCellFree(x, y)) {
        return false;
    }
    
    grid[y][x] = Cell('X', true, false);
    return true;
}

void PlayingField::AddTower(const EnemyTower& tower) 
{
    int x = tower.GetX();
    int y = tower.GetY();
    
    if (!IsValidPosition(x, y)) {
        throw invalid_argument("Cannot place tower at invalid position: (" + to_string(x) + ", " + to_string(y) + ")");
    }
    
    if (grid[y][x].IsWall()) {
        throw invalid_argument("Cannot place tower on wall");
    }
    
    if (grid[y][x].GetSymbol() == 'X') {
    }
    
    towers.push_back(tower);
    SetCellSymbol(x, y, 'W', true);
}

vector<EnemyTower>& PlayingField::GetTowers() 
{
    return towers;
}

const vector<EnemyTower>& PlayingField::GetTowers() const 
{
    return towers;
}

void PlayingField::UpdateTowers() 
{
    for (auto& tower : towers) {
        tower.Update();
    }
}

void PlayingField::CheckTowerAttacks(Player& player) 
{
    for (auto& tower : towers) 
    {
        if (tower.IsPlayerInRange(player)) 
        {
            tower.AttackPlayer(player);
        }
    }
}

void PlayingField::DisplayTowers() const 
{
    if (towers.empty()) 
    {
        return;
    }

    for (size_t i = 0; i < towers.size(); ++i) 
    {
        cout << "Tower " << i << ": ";
        towers[i].DisplayInfo();
    }
}

EnemyTower* PlayingField::GetNearestTower(int playerX, int playerY) 
{
    if (towers.empty()) return nullptr;
    
    EnemyTower* nearest = &towers[0];
    int minDistance = abs(nearest->GetX() - playerX) + abs(nearest->GetY() - playerY);
    
    for (auto& tower : towers) 
    {
        int distance = abs(tower.GetX() - playerX) + abs(tower.GetY() - playerY);
        if (distance < minDistance) 
        {
            minDistance = distance;
            nearest = &tower;
        }
    }
    return nearest;
}

bool PlayingField::IsBarrierAt(int x, int y) const 
{
    if (!IsValidPosition(x, y)) return false;
    return grid[y][x].GetSymbol() == 'X';
}

void PlayingField::DamageTowersInRange(int centerX, int centerY, int range, int damage) 
{
    for (auto& tower : towers) {
        int towerX = tower.GetX();
        int towerY = tower.GetY();
        int distance = abs(towerX - centerX) + abs(towerY - centerY);
        
        if (distance <= range) {
            tower.TakeDamage(damage);
        }
    }
}

void PlayingField::RestoreBarriers(const vector<pair<int, int>>& barrierPositions) 
{
    for (int y = 1; y < height - 1; ++y) {
        for (int x = 1; x < width - 1; ++x) {
            if (grid[y][x].GetSymbol() == 'X') {
                grid[y][x] = Cell('.', false, false);
            }
        }
    }
    
    for (const auto& pos : barrierPositions) {
        int x = pos.first;
        int y = pos.second;
        
        if (x >= 0 && x < width && y >= 0 && y < height) {
            if (!grid[y][x].IsWall()) {
                bool hasTower = false;
                for (const auto& tower : towers) {
                    if (tower.GetX() == x && tower.GetY() == y) {
                        hasTower = true;
                        break;
                    }
                }
                
                if (!hasTower) {
                    grid[y][x] = Cell('X', true, false);
                }
            }
        }
    }
}

EnemyTower* PlayingField::AddTower(int x, int y) 
{
    if (!IsValidPosition(x, y)) 
    {
        throw invalid_argument("Cannot place tower at invalid position");
    }
    
    if (grid[y][x].IsWall()) 
    {
        throw invalid_argument("Cannot place tower on wall");
    }
    
    if (grid[y][x].GetSymbol() == 'X') {
    }
    
    towers.push_back(EnemyTower(x, y, 100, 100, 15, 3));
    SetCellSymbol(x, y, 'W', true);
    return &towers.back();
}

int PlayingField::GetBarrierCount() const 
{
    int barrierCount = 0;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (grid[y][x].GetSymbol() == 'X') {
                barrierCount++;
            }
        }
    }
    return barrierCount;
}

vector<pair<int, int>> PlayingField::GetBarrierCoordinates() const 
{
    vector<pair<int, int>> barrierCoords;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (grid[y][x].GetSymbol() == 'X') {
                barrierCoords.push_back({x, y});
            }
        }
    }
    return barrierCoords;
}

bool PlayingField::HasBarrierAt(int x, int y) const {
    return IsBarrierAt(x, y);
}

bool PlayingField::HasTowerAt(int x, int y) const {
    for (const auto& tower : towers) {
        if (tower.GetX() == x && tower.GetY() == y) {
            return true;
        }
    }
    return false;
}
