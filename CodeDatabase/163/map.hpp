#ifndef MAP_HPP
#define MAP_HPP

#include <memory>
#include <vector>
#include <string>
#include "cell.hpp"

class Map {
private:
    int length;
    int width;
    std::vector<std::vector<std::unique_ptr<Cell>>> board;

public:
    Map(int length = 15, int width = 15);
    ~Map() = default;
    
    Map(const Map&) = delete;
    Map& operator=(const Map&) = delete;
    Map(Map&& other) noexcept = default;
    Map& operator=(Map&& other) noexcept = default;
    
    const Cell& getCell(int x, int y) const;
    Cell& getCell(int x, int y);
    int getLength() const;
    int getWidth() const;
    bool isValidPosition(int x, int y) const;
    bool isCellPassable(int x, int y) const;
    void setCellType(int x, int y, Cell::Type type);
    void printMap() const;
    
private:
    void initializeBoard();
};

#endif