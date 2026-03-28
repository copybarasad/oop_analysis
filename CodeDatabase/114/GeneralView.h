#ifndef GENERALVIEW_H
#define GENERALVIEW_H

#include <string>
#include <vector>
#include "GameField.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyBuilding.h"

class GeneralView {
public:
    void printField(GameField& field) const;
    void printPlayer(const Player& player) const;
    void printEnemies(const std::vector<Enemy>& enemies) const;
    void printBuildings(const std::vector<EnemyBuilding>& buildings) const;
    void print(const std::string& message) const;
};

#endif
