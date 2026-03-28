#include "FightManager.h"
#include <cmath>

double FightManager::SearchDistance(int playerX, int playerY, const std::pair<int,int>& enemyPos){
    int dx = enemyPos.first - playerX;
    int dy = enemyPos.second - playerY;
    return std::sqrt(dx * dx + dy * dy);
}

std::vector<EnemyInfo> FightManager::SearchEnemy(int playerX, int playerY, const std::vector<std::pair<int,int>>& enemyPositions, const std::vector<Enemy>& enemies){
    std::vector<EnemyInfo> foundEnemies;
    const int radius = 6;
    
    for (size_t i = 0; i < enemies.size(); i++){
        double distan = SearchDistance(playerX, playerY, enemyPositions[i]);

        if (distan <= radius){
            EnemyInfo info;
            info.id = i + 1;
            info.x = enemyPositions[i].first;
            info.y = enemyPositions[i].second;
            info.distance = distan;
            info.health = enemies[i].GetHealth();
            foundEnemies.push_back(info);
        }
    }
    
    return foundEnemies;
}

AttackResult FightManager::Fight(Player& player, std::vector<Enemy>& enemies, std::vector<std::pair<int,int>>& enemyPositions,  int playerX, int playerY){
    AttackResult result;
    int attackmod = (player.GetAttackType() == AttackType::RANGED) ? 6 : 2;
    int enemyindex = -1;

    for(size_t i = 0; i < enemies.size(); i++){
        double distance = SearchDistance(playerX, playerY, enemyPositions[i]);
        if(distance <= attackmod){
            enemyindex = static_cast<int>(i);
            break;
        }
    }
    
    if (enemyindex == -1){
        result.message = "Врагов поблизости нет, не с кем сразиться";
        return result;
    }

    Enemy& enemy = enemies[enemyindex];
    std::string fightLog = "Ты вступаешь в бой с " + std::to_string(enemyindex + 1) + " врагом\n";

    int countfight = 0;
    while(player.IsLife() && enemy.IsLife()){
        fightLog += "Раунд " + std::to_string(countfight) + ": ";
        fightLog += "Твое здоровье " + std::to_string(player.GetHealth()) + ", ";
        fightLog += "Здоровье врага " + std::to_string(enemy.GetHealth()) + "\n";

        player.PlayerAttack(enemy);
        countfight++;
        
        if(!enemy.IsLife()){
            enemies.erase(enemies.begin() + enemyindex);
            enemyPositions.erase(enemyPositions.begin() + enemyindex);
            result.enemyDefeated = true;
            result.message = fightLog + "Враг побежден";
            result.playerHealthAfter = player.GetHealth();
            return result;
        }

        enemy.EnemyAttack(player);
        if (!player.IsLife()) {
            result.playerDefeated = true;
            result.message = fightLog + "Ты проиграла";
            return result;
        }
    }
    
    return result;
}

std::vector<std::string> FightManager::EnemiesAttackIfPlayerNear(Player& player,  std::vector<Enemy>& enemies, const std::vector<std::pair<int,int>>& enemyPositions,  int playerX, int playerY){
    std::vector<std::string> attacks;
    
    for (size_t i = 0; i < enemies.size(); i++){
        double distan = SearchDistance(playerX, playerY, enemyPositions[i]);

        if(distan <= 6){
            bool playerDied = enemies[i].EnemyAttack(player);
            attacks.push_back("Враг " + std::to_string(i + 1) + " атаковал тебя");
            
            if(playerDied){
                attacks.push_back("Враг убил тебя");
                return attacks;
            }
        }
    }
    
    return attacks;
}

std::string FightManager::SwitchPlayerAttackType(Player& player){
    player.SwitchAttackType();
    std::string attackType = (player.GetAttackType() == AttackType::MELEE) ? "ближний бой" : "дальний бой";
    return "Тип атаки изменен. Теперь это " + attackType;
}