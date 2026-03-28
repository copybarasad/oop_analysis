#ifndef ENTITY_H
#define ENTITY_H

#include <map>
#include <utility>
#include <vector>
#include <cstdint>
#include <string>
class Map;  

class Entity {
protected:
    std::map<char, std::pair<int, int>> directions = {
        {'u', {-1, 0}}, {'l', {0, -1}}, {'d', {1, 0}}, {'r', {0, 1}}
    };

public:
    int x, y;
    int heallpoints, damage;
    char ind_type;

    virtual void Spawn(int x, int y, Map& map) = 0;
    virtual void Move(Map& map) = 0;
    virtual std::vector<uint8_t> Save() const = 0;
    virtual void Load(const std::vector<uint8_t>& data) = 0;
    virtual size_t GetSaveSize() const = 0;
    virtual void Slowed() = 0;
    void SetCoord(int new_x, int new_y);
    void GetDirection(char dir, int& dx, int& dy) const;
    void TakeDamage(int damage, Map& map);
    virtual std::string SaveToString() const;
    virtual void LoadFromString(const std::string& data);
    virtual std::string GetSaveHeader() const;
};

#endif