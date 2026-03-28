#pragma once
#include <string>
#include <tuple>

class EnemyBase {
private:
    int spawnInterval_;
    int turnsUntilSpawn_;
    int hp_;  
    int maxHp_;

public:
    explicit EnemyBase(int interval = 3, int hp = 50);
    void tick();
    bool canSpawn() const;
    void resetTimer();
    int getSpawnInterval() const;
    
    int getHp() const;
    int getMaxHp() const;
    bool isAlive() const;
    void takeDamage(int damage);
    
    void setHp(int hp) { hp_ = hp; }
    void setSpawnInterval(int interval) { spawnInterval_ = interval; }
    void setTurnsUntilSpawn(int turns) { turnsUntilSpawn_ = turns; }
    int getTurnsUntilSpawn() const { return turnsUntilSpawn_; }
    static std::tuple<int, int, int, int> parseData(const std::string& data);
};