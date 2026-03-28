#include "Grid.hpp"
#include <stdexcept>
#include <random>
#include <ctime>


Grid::Grid(int x, int y) {
    if (x < 10 || x > 25 || y < 10 || y > 25)
        throw std::invalid_argument("grid range from 10x10 to 25x25 only allowed!");
    width_ = x;
    height_ = y;
    std::mt19937 rng(static_cast<unsigned>(std::time(nullptr)));
    std::uniform_real_distribution<double> dist(0.0,1.0);

    grid.resize(height_);
    for (auto& row : grid)
        row.resize(width_);

    for (int i = 0; i < height_; i++) {
        for (int j = 0; j < width_; j++) {
	    if(i ==0 && j ==0 ) continue;
	    double r = dist(rng);

	    if (r < 0.05)
		grid[i][j] = cellType::forbidden;
	    else if(r < 0.15)
		grid[i][j] = cellType::slowed;
	    else
		grid[i][j] = cellType::free;
        }
    }
}
Grid::Grid(const Grid& other) = default;
Grid& Grid::operator=(const Grid& other) = default;
Grid::Grid(Grid&& other) = default;
Grid& Grid::operator=(Grid&& other) = default;


bool Grid::IsFreeCell(int x, int y) const {
    if (x < 0 || x >= width_ || y < 0 || y >= height_) return false;
    return grid[x][y] != cellType::forbidden;
}

bool Grid::slowedCell(int x, int y){
    return grid[x][y] == cellType::slowed;
}

void Grid::setObj(int x, int y, cellType type) {
    if (x >= 0 && x < width_ && y >= 0 && y < height_)
        grid[x][y] = type;
}

cellType Grid::getCell(int x, int y) const {
    if (x < 0 || x >= width_ || y < 0 || y >= height_) return cellType::forbidden;
    return grid[x][y];
}

int Grid::getWidth() const { return width_; }
int Grid::getHeight() const { return height_; }



void Grid::save(std::ostream& os) const {
    os << width_ << " " << height_ << "\n";
    for (const auto& row : grid) {
        for (const auto& cell : row) {
            os << static_cast<int>(cell) << " ";
        }
        os << "\n";
    }
}

void Grid::load(std::istream& is) {
    int w, h;
    is >> w >> h;
    
    if (w != width_ || h != height_) {
        width_ = w;
        height_ = h;
        grid.assign(height_, std::vector<cellType>(width_));
    }

    int val;
    for (int i = 0; i < height_; ++i) {
        for (int j = 0; j < width_; ++j) {
            is >> val;
            grid[i][j] = static_cast<cellType>(val);
        }
    }
}
