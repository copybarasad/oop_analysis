#ifndef RPG_GAMEFIELD_H
#define RPG_GAMEFIELD_H

#include "Cell.h"
#include <vector>
#include <random>
#include <utility>
#include <set>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

typedef struct {
    std::pair<int, int> markPosition;
    int markDiameter;
    std::pair<int, int> areaPosition;
    int areaDiameter;
} VisualData;

class GameField {
    unsigned int width;
    unsigned int height;
    float wallChance;
    float slowChance;
    std::vector<std::vector<Cell>> cells;
    std::mt19937 gen;
public:
    GameField(std::pair<unsigned int,unsigned int> size, float wallChance, float slowChance);

    explicit GameField(json& data);

    ~GameField() = default;

    void uniteSeparatedAreas();

    GameField(const GameField& other);

    GameField& operator=(const GameField& other);

    GameField(GameField&& other) noexcept;

    GameField& operator=(GameField&& other) noexcept;

    [[nodiscard]] json to_json() const;

    [[nodiscard]] std::vector<std::pair<int,int>> getPassableCells() const;

    [[nodiscard]] bool isInBounds(const std::pair<int, int>& coordinates) const;

    [[nodiscard]] bool areCoordinatesInArea(const std::pair<int, int>& coordinates, const std::pair<int, int>& area, const int& areaDiameter) const;

    [[nodiscard]] Cell& getCell(const std::pair<int, int>& coordinates);

    std::pair<int,int> getFreeCellCoordinates();

    [[nodiscard]] int getHeight() const;

    [[nodiscard]] int getWidth() const;

    [[nodiscard]] std::mt19937& getRandomGenerator();

    int collectPoints();

    void expand();

    void generate();
};

#endif
