#pragma once
#include <vector>
#include <string>

class Enemy;
class Field;
class EnemyBuilding;

class Spell {
public:
    virtual ~Spell() = default;

    virtual std::string GetName() const = 0;
    virtual bool Use(std::vector<Enemy>& enemies, std::vector<EnemyBuilding>& enemy_buildings,
                     int px, int py, Field& field) = 0;
};