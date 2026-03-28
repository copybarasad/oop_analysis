#pragma once

#include <vector>
#include "Player.h"
#include "Enemy.h"
#include "Cell.h"

class GameField {
public:
    void SetSize(int size);
    GameField(int size);
    void InitializeField();
    void PlacePlayer(Player* player, int x, int y);
    void MovePlayer(int dx, int dy);
    void PlaceEnemy(Enemy* enemy, int x, int y);
    void MoveEnemy(int index, int dx, int dy);
    void RemoveEnemy(Enemy* enemy);

    bool IsInside(int x, int y) const;
    bool IsCellFree(int x, int y) const;
    bool GameIsOver() const;
    void CheckAndApplyDamage(int x, int y);
    void DamagePlayer(int damage);
    std::vector<Enemy*> GetEnemies() const;
    void DisplayField() const;
    int GetEnemiesCount() const;
    int GetSize() const;

    GameField(const GameField& other);
    GameField& operator=(const GameField& other);
    GameField(GameField&& other) noexcept;
    GameField& operator=(GameField&& other) noexcept;
    ~GameField();
    void Clear();


private:
    int fieldSize;
    std::vector<std::vector<Cell>> field;
    Player* playerPtr;
    std::vector<Enemy*> enemies;
};
