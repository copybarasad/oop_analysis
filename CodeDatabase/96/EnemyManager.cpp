#include <utility>
#include <map>
#include <cstdlib>
#include "EnemyManager.h"
#include "Enemy.h"
#include "Entity.h"
#include "PlayerManager.h"
#include <iostream>
#include <set>

EnemyManager::EnemyManager() = default;
EnemyManager::~EnemyManager() = default;


EnemyManager::EnemyManager(dataEnemyManager data){
    
    for(auto e:data.enemies){
        AddEnemyAtPos(e.health, e.damage, e.pos);
        enemies[e.pos].SetStunDuration(e.stunDuration);
    }
}




void EnemyManager::AddEnemyAtPos(int health, int damage, std::pair<int,int> pos) {
    Enemy enemy(health, damage);
    enemies.emplace(pos, enemy);
}

void EnemyManager::RemoveEnemyAtPos(std::pair<int,int> pos) {
    enemies.erase(pos);
}

int EnemyManager::GetEnemyDamageAtPos(std::pair<int,int> pos) const {
    auto it = enemies.find(pos);
    if (it == enemies.end()) return 0;
    return it->second.GetDamage();
}

void EnemyManager::TakeEnemyDamageAtPos(std::pair<int,int> pos, int amount) {
    auto it = enemies.find(pos);
    if (it == enemies.end()) return;
    it->second.TakeDamage(amount);
}

bool EnemyManager::IsEnemyAtPos(std::pair<int,int> pos) const {
    return enemies.find(pos) != enemies.end();
}


const std::vector<std::pair<int,int>> EnemyManager::MoveEnemies(const Field& f) {
    std::vector<std::pair<int,int>> positions;
    std::vector<std::pair<std::pair<int,int>, std::pair<int,int>>> moves;
    
    // Множество занятых позиций (текущих + планируемых)
    std::set<std::pair<int,int>> occupiedPositions;
    
    // Изначально все текущие позиции заняты
    for (auto& [pos, enemy] : enemies) {
        occupiedPositions.insert(pos);
    }
    
    positions.reserve(enemies.size() * 2);
    
    // Сначала собираем все планируемые перемещения
    for (auto& [pos, enemy] : enemies) {
        auto newPos = f.FindFreeAdjacentPosition(pos);
        
        // Проверяем, что позиция свободна И не занята другими планируемыми перемещениями
        if (newPos != pos && 
            enemies.find(newPos) == enemies.end() && 
            occupiedPositions.find(newPos) == occupiedPositions.end()) {
            
            if(f.IsOccupiedAtPos(newPos)) {
                positions.push_back(pos);
                positions.push_back(newPos);
            } else {
                positions.push_back(pos);
                positions.push_back(newPos);
                moves.emplace_back(pos, newPos);
                // Помечаем новую позицию как занятую
                occupiedPositions.insert(newPos);
            }
        } else {
            // Не можем переместиться
            positions.push_back(pos);
            positions.push_back(pos);
        }
    }
    
    // Применяем перемещения
    for (auto& [oldPos, newPos] : moves) {
        auto it = enemies.find(oldPos);
        if (it != enemies.end()) {
            enemies.emplace(newPos, std::move(it->second));
            enemies.erase(it);
        }
    }
    
    return positions;
}


dataEnemyManager EnemyManager::Save(){
    dataEnemyManager data;
    data.numEnemies=enemies.size();
    for (auto& [pos, enemy] : enemies) {
        dataEnemy datae = enemy.Save();
        datae.pos=pos;
        data.enemies.push_back(datae);
    }
    return data;
}