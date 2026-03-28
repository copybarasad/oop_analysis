#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"
#include <vector>
#include <cstdint>
#include <string>

class Enemy : public Entity {
private:
    static const char traffic_map[4];
    int traffic_ind = 0;
    static const size_t SAVE_SIZE = 4 * sizeof(int) + sizeof(bool);

public:
    bool is_slowed = false;
    int damage = 10;  

    Enemy();
    Enemy(int health, int damage);

    void Slowed() override;
    void Spawn(int x, int y, Map& map) override;
    void Move(Map& map) override;

    bool IsSlowed();
    int GetTrafficInd() const { return traffic_ind; }
    void SetTrafficInd(int value) { traffic_ind = value; }
    bool GetIsSlowed() const { return is_slowed; }

    std::vector<uint8_t> Save() const override;
    void Load(const std::vector<uint8_t>& data) override;
    size_t GetSaveSize() const override;
    std::string SaveToString() const override;
    void LoadFromString(const std::string& data) override;
    std::string GetSaveHeader() const override;

    void LogCurrentState() const;
    std::string GetDebugInfo() const;

    void TakeDamage(int damage, Map& map);

    Enemy(const Enemy&) = delete;
    Enemy& operator=(const Enemy&) = delete;
};

#endif 