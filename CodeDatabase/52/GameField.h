#pragma once
#include <vector>
#include <memory>
#include <utility>
#include "Cell.h"
#include "Player.h"
#include "Entity.h"


class Player;
class Enemy;
class Ally;
class Entity;

class GameField {
    int width_;
    int height_;
    std::vector<std::vector<Cell>> cells_;
    std::shared_ptr<Player> player_;
public:
    GameField(int w, int h);

    int Width() const noexcept { return width_; }
    int Height() const noexcept { return height_; }

    bool Inside(int x, int y) const noexcept;
    Cell& CellAt(int x, int y);
    const Cell& CellAt(int x, int y) const;

    void PlacePlayer(std::shared_ptr<Player> p, int x, int y);
    std::pair<int,int> PlayerCell() const;
    std::pair<int,int> FindEntity(const Entity* who) const;

    Enemy* EnemyAt(int x, int y) const noexcept;
    Ally* AllyAt(int x, int y) const noexcept;

    bool TryMove(Entity* who, int dx, int dy);
    void PlaceTrap(int x, int y, int dmg);
    void SpawnAlly(int x, int y);
    void SpawnAlliesAround(int cx, int cy, int count);
    void KillDead();

    std::vector<std::pair<int,int>> ListEnemies() const;
    bool AnyEnemiesAlive() const;

    void UpdateAll();

    void GenerateWalls(float density);
    bool PlayerHasPath() const;

    void ClearCell(int x, int y) {
        cells_[y][x].Clear();
    }

    void ClearAll() {
        for (auto& row : cells_)
            for (auto& c : row)
                c.Clear();
    }

    void SetEntity(int x, int y, std::shared_ptr<Entity> e);

    void SetTrap(int x, int y, int dmg);

    void SetPlayerPosition(int x, int y) {
        auto [px, py] = PlayerCell();
        cells_[py][px].setEntity(nullptr);
        cells_[y][x].setEntity(std::static_pointer_cast<Entity>(player_));
    }

    void SpawnTower(int x, int y);

};