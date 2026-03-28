#ifndef GAME_MAP_H
#define GAME_MAP_H

#include <vector>
#include "Cell.h"
#include "Coordinates.h"

class Object;

class Map {
private:
    std::vector<std::vector<Cell> > cells_;
    size_t width_, height_, layers_;

public:
    Map(size_t width, size_t height, size_t layers);

    Map(size_t width, size_t height, size_t layers, std::vector<Coordinates> blocks);

    Map(const Map &other)
        : cells_(other.cells_),
          width_(other.width_),
          height_(other.height_),
          layers_(other.layers_) {
    };

    Map &operator=(const Map &other) {
        if (this != &other) {
            width_ = other.width_;
            height_ = other.height_;
            layers_ = other.layers_;
            cells_ = other.cells_;
        }
        return *this;
    }

    Map(Map &&other) noexcept
        : width_(0), height_(0), layers_(0) {
        std::swap(width_, other.width_);
        std::swap(height_, other.height_);
        std::swap(layers_, other.layers_);
        std::swap(cells_, other.cells_);
    }

    Map &operator=(Map &&other) noexcept {
        if (this != &other) {
            std::swap(width_, other.width_);
            std::swap(height_, other.height_);
            std::swap(layers_, other.layers_);
            std::swap(cells_, other.cells_);
        }
        return *this;
    }


    Coordinates getSize() const;

    Object *getFromCell(const Coordinates &pos) const;

    bool isOccupied(const Coordinates &pos) const;

    int addToCell(const Coordinates &pos, Object *obj);

    void removeFromCell(const Coordinates &pos);

    int moveFromTo(const Coordinates &from, const Coordinates &to);
};

#endif
