#include <vector>
#include <utility>
#include <set>
#include "map"
#include "AllyManager.h"


AllyManager::AllyManager() = default;
AllyManager::~AllyManager() = default;

AllyManager::AllyManager(dataAllyManager data){
    for(auto a:data.Allies){
        allies.emplace(a.pos, Ally(a));
    }
}

void AllyManager::AddAllyAtPos(int health, int damage, std::pair<int,int> pos) {
    allies.emplace(pos, Ally(health, damage));
}

void AllyManager::RemoveAllyAtPos(std::pair<int,int> pos) {
    allies.erase(pos);
}

bool AllyManager::IsAllyAtPos(std::pair<int,int> pos) const {
    return allies.find(pos) != allies.end();
}

void AllyManager::TakeAllyDamageAtPos(std::pair<int,int> pos, int amount) {
    auto it = allies.find(pos);
    if (it != allies.end()) {
        it->second.TakeDamage(amount);
    }
}

int AllyManager::GetAllyDamageAtPos(std::pair<int,int> pos) const {
    auto it = allies.find(pos);
    if (it != allies.end()) {
        return it->second.GetDamage();
    }
    return 0;
}

const std::vector<std::pair<int,int>> AllyManager::MoveAllys(const Field& f) {
    std::vector<std::pair<int,int>> positions;
    std::vector<std::pair<std::pair<int,int>, std::pair<int,int>>> moves; // (oldPos, newPos)
    
    // Множество занятых позиций (текущих + планируемых)
    std::set<std::pair<int,int>> occupiedPositions;
    
    // Изначально все текущие позиции заняты
    for (auto& [pos, ally] : allies) {
        occupiedPositions.insert(pos);
    }
    
    positions.reserve(allies.size() * 2);
    
    // Сначала собираем все планируемые перемещения
    for (auto& [pos, ally] : allies) {
        auto newPos = f.FindFreeAdjacentPosition(pos);
        
        // Проверяем, что позиция свободна И не занята другими планируемыми перемещениями
        if (newPos != pos && allies.find(newPos) == allies.end() && 
            occupiedPositions.find(newPos) == occupiedPositions.end()) {
            
            if(f.IsOccupiedAtPos(newPos)) {
                // Позиция занята на поле - просто добавляем в вывод
                positions.push_back(pos);
                positions.push_back(newPos);
            } else {
                // Свободная позиция - планируем перемещение
                positions.push_back(pos);
                positions.push_back(newPos);
                moves.emplace_back(pos, newPos);
                // Помечаем новую позицию как занятую для следующих союзников
                occupiedPositions.insert(newPos);
            }
        } else {
            // Не можем переместиться
            positions.push_back(pos);
            positions.push_back(pos);
        }
    }
    
    // Теперь применяем все перемещения
    for (auto& [oldPos, newPos] : moves) {
        auto it = allies.find(oldPos);
        if (it != allies.end()) {
            allies.emplace(newPos, std::move(it->second));
            allies.erase(it);
        }
    }
    
    return positions;
}

dataAllyManager AllyManager::Save(){
    dataAllyManager data;
    data.numAllies=allies.size();
    for (auto& [pos, ally] : allies) {
        dataAlly datae = ally.Save();
        datae.pos=pos;
        data.Allies.push_back(datae);
    }
    return data;
}