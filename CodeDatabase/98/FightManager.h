
#ifndef FIGHTMANAGER_H
#define FIGHTMANAGER_H

#include <vector>
#include <string>
#include "player.h"
#include "enemy.h"
#include "AttackType.h"
#include "field.h"
#include "GameTypes.h"

class FightManager{
public:
    static double SearchDistance(int playerX, int playerY, const std::pair<int,int>& enemyPos);
    static std::vector<EnemyInfo> SearchEnemy(int playerX, int playerY, const std::vector<std::pair<int,int>>& enemyPositions, const std::vector<Enemy>& enemies);
    static AttackResult Fight(Player& player, std::vector<Enemy>& enemies,  std::vector<std::pair<int,int>>& enemyPositions,  int playerX, int playerY);
    static std::vector<std::string> EnemiesAttackIfPlayerNear(Player& player,  std::vector<Enemy>& enemies,  const std::vector<std::pair<int,int>>& enemyPositions,  int playerX, int playerY);
    static std::string SwitchPlayerAttackType(Player& player);
};

#endif