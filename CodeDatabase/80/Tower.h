#ifndef TOWER_H
#define TOWER_H

#include "Entity.h"
#include <vector>
#include <cstdint>
#include <string>

class Tower : public Entity {
    int move_num;
    int spawn_move;
    int enemyHealth;    
    int enemyDamage;    
    static const size_t SAVE_SIZE = 4 * sizeof(int);

public:
    Tower(int spawn_move);
    Tower(int spawn_move, int eHealth, int eDamage);
    void SetEnemyStats(int health, int damage);
    void Slowed() override;
    void Spawn(int x, int y, Map& map) override;
    void Move(Map& map) override;
    std::vector<uint8_t> Save() const override;
    void Load(const std::vector<uint8_t>& data) override;
    size_t GetSaveSize() const override;
    std::string SaveToString() const override;
    void LoadFromString(const std::string& data) override;
    std::string GetSaveHeader() const override;

    int GetMoveNum() const { return move_num; }
    void SetMoveNum(int value) { move_num = value; }
    int GetSpawnMove() const { return spawn_move; }
};

#endif