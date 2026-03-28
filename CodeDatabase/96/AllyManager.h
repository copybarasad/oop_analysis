#pragma once
#include <vector>
#include <utility>
#include "map"
#include "Ally.h"
#include "Field.h"
#include "struct/dataAllyManager.h"

class AllyManager {
private:
    std::map<std::pair<int,int>, Ally> allies;
public:
    AllyManager();
    ~AllyManager();

    AllyManager(dataAllyManager data);
    void AddAllyAtPos(int health, int damage, std::pair<int,int> pos);
    void RemoveAllyAtPos(std::pair<int,int> pos);
    bool IsAllyAtPos(std::pair<int,int> pos) const;
    void TakeAllyDamageAtPos(std::pair<int,int> pos, int amount);
    int GetAllyDamageAtPos(std::pair<int,int> pos) const;
    const std::map<std::pair<int,int>, Ally>& GetAllies() const { return allies; }
    const std::vector<std::pair<int,int>> MoveAllys(const Field& f);

    dataAllyManager Save();
};