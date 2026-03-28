#pragma once
#include <../nlohmann/json.hpp>

class Cell {
public:
    enum class CellType {
        EMPTY,
        BUILDING,
        FORTRESS,
        WALL,
        ENEMY,
        PLAYER,
        TRAP,
        ALLY
    };
private:
    CellType cellType;
    bool slowdown;
    
public:
    Cell(CellType cellType = CellType::EMPTY);
    ~Cell() = default;
    CellType getType() const;
    void setType(CellType type);
    bool isSlowing() const;
    void setSlowing();
    nlohmann::json toJson() const;
    void fromJson(const nlohmann::json& j);
};