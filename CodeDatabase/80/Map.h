#ifndef MAP_H
#define MAP_H

#include "Entity.h"
#include <vector>
#include <cstdint>
#include <map>
#include <utility>
#include <string>

class Entity;

class Map {
    int weight;
    int height;
    std::vector<std::vector<char>> map_arr;
    std::map<std::pair<int, int>, Entity*> obj_map;
    static const size_t MAP_HEADER_SIZE = 2 * sizeof(int);
    static const std::string MAP_HEADER;

public:
    Map(int weight, int height);
    Map(const Map& other);
    Map(Map&& other) noexcept;
    Map& operator=(const Map& other);
    Map& operator=(Map&& other) noexcept;
    void SetVal(int x, int y, char val, Entity* obj);
    char GetVal(int x, int y);
    void SeeMap();
    bool CanMoveTo(int x, int y);
    std::map<std::pair<int, int>, Entity*>& GetObjects();
    int GetWeight() const;
    int GetHeight() const;
    std::vector<uint8_t> Save() const;
    void Load(const std::vector<uint8_t>& data);
    size_t GetMapSaveSize() const;
    const std::map<std::pair<int, int>, Entity*>& GetAllObjects() const { return obj_map; }
    void ClearAllObjects();
    void CheckTraps(int x, int y, Entity* entity);
    std::string SaveToString() const;
    void LoadFromString(const std::string& data);
    std::string GetMapHeader() const;
};

#endif