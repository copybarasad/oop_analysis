#pragma once
#include <vector>
#include <iostream>

enum class cellType { free, slowed, forbidden };

class Grid {
    int width_;
    int height_;
    std::vector<std::vector<cellType>> grid;

public:
    Grid(int x, int y);
    Grid(const Grid& other) ;
    Grid& operator=(const Grid& other) ;
    Grid(Grid&&);
    Grid& operator=(Grid&&);


    bool IsFreeCell(int x, int y) const;
    bool slowedCell(int x, int y);
    void setObj(int x, int y, cellType type = cellType::forbidden);
    cellType getCell(int x, int y) const;

    int getWidth() const;
    int getHeight() const;


    void save(std::ostream& os) const;
    void load(std::istream& is);
};
