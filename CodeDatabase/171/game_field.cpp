#include "game_field.h"
#include <cstdlib>
#include <ctime>

GameField::GameField(int width, int height)
    : width_(width), height_(height) {
    if (width < 10 || width > 25 || height < 10 || height > 25) {
        width_ = 15;
        height_ = 15;
    }
    initializePoints();
}

GameField::GameField(const GameField& other)
    : width_(other.width_), height_(other.height_) {
    copyFrom(other);
}

GameField::GameField(GameField&& other) noexcept
    : width_(other.width_), height_(other.height_), points_(other.points_) {
    other.points_ = nullptr;
}

GameField& GameField::operator=(const GameField& other) {
    if (this != &other) {
        cleanup();
        width_ = other.width_;
        height_ = other.height_;
        copyFrom(other);
    }
    return *this;
}

GameField& GameField::operator=(GameField&& other) noexcept {
    if (this != &other) {
        cleanup();
        width_ = other.width_;
        height_ = other.height_;
        points_ = other.points_;
        other.points_ = nullptr;
    }
    return *this;
}

GameField::~GameField() {
    cleanup();
}

void GameField::initializePoints() {
    points_ = new Point * [height_];
    for (int i = 0; i < height_; ++i) {
        points_[i] = new Point[width_];
        for (int j = 0; j < width_; ++j) {
            bool passable = (rand() % 5) != 0;
            points_[i][j] = Point(passable);
        }
    }
}

void GameField::copyFrom(const GameField& other) {
    points_ = new Point * [height_];
    for (int i = 0; i < height_; ++i) {
        points_[i] = new Point[width_];
        for (int j = 0; j < width_; ++j) {
            points_[i][j] = other.points_[i][j];
        }
    }
}

void GameField::cleanup() {
    if (points_) {
        for (int i = 0; i < height_; ++i) {
            delete[] points_[i];
        }
        delete[] points_;
    }
}

bool GameField::isPositionValid(Position pos) const {
    return pos.x >= 0 && pos.x < width_ && pos.y >= 0 && pos.y < height_;
}

bool GameField::isPositionPassable(Position pos) const {
    return isPositionValid(pos) && points_[pos.y][pos.x].isPassable();
}

int GameField::getWidth() const {
    return width_;
}

int GameField::getHeight() const {
    return height_;
}