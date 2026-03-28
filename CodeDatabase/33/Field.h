#pragma once
#include <vector>
#include <random>
#include "Cell.h"
#include "Coord.h"
#define MIN_SIZE 10
#define MAX_SIZE 25

class Field {
public:
    static constexpr int kMin = MIN_SIZE;
    static constexpr int kMax = MAX_SIZE;

    Field();
    Field(int w, int h, std::mt19937& rng);

    Field(const Field& other);
    Field(Field&& other);
    Field& operator=(const Field& other);
    Field& operator=(Field&& other);
    ~Field() = default;

    int W() const;
    int H() const;

    bool Inside(Coord p) const;
    const Cell& At(Coord p) const;
    Cell& At(Coord p);

    bool IsPassable(Coord p) const;

    void Render(const Coord& player,
                const std::vector<Coord>& enemies,
                const std::vector<Coord>& allies,
                const std::vector<Coord>& towers,
                const std::vector<Coord>& turrets,
                const std::vector<Coord>& traps) const;

    Coord RandomPassable(std::mt19937& rng) const;

private:
    int w_;
    int h_;
    std::vector<Cell> cells_;

    int Idx(int x, int y) const;
    void Generate(std::mt19937& rng);
};
