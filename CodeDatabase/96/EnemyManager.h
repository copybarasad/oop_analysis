#pragma once
#include <utility>
#include <map>
#include <memory>
#include <functional>
#include "Enemy.h"
#include "Field.h"
#include "struct/dataEnemyManager.h"


class PlayerManager;



class EnemyManager {
private:
    std::map<std::pair<int,int>, Enemy> enemies;
public:
    EnemyManager();
    EnemyManager(dataEnemyManager data);
    ~EnemyManager();
    void AddEnemyAtPos(int health, int damage, std::pair<int,int> pos);
    void RemoveEnemyAtPos(std::pair<int,int> pos);
    int GetEnemyDamageAtPos(std::pair<int,int> pos) const;
    void TakeEnemyDamageAtPos(std::pair<int,int> pos, int amount);
    bool IsEnemyAtPos(std::pair<int,int> pos) const;
    const std::vector<std::pair<int,int>> MoveEnemies(const Field& f);
    const std::map<std::pair<int,int>, Enemy>& GetEnemies() const { return enemies; }
    dataEnemyManager Save();

    
};