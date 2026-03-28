#include "Field.h"
#include <algorithm>
#include <stdexcept>
#include <iostream>
#define LIMIT 5000
#define MINER_COUNT 3

Field::Field() : w_(0), h_(0) {}

Field::Field(int w, int h, std::mt19937& rng)
    : w_(w), h_(h), cells_(w * h, Cell(Cell::CellType::Wall)) {
    Generate(rng);
}

Field::Field(const Field& o)
    : w_(o.w_), h_(o.h_), cells_(o.cells_) {}

Field::Field(Field&& o)
    : w_(o.w_), h_(o.h_), cells_(std::move(o.cells_)) {
    o.w_ = 0;
    o.h_ = 0;
}

Field& Field::operator=(const Field& o) {
    if (this != &o) {
        w_ = o.w_;
        h_ = o.h_;
        cells_ = o.cells_;
    }
    return *this;
}

Field& Field::operator=(Field&& o) {
    if (this != &o) {
        w_ = o.w_;
        h_ = o.h_;
        cells_ = std::move(o.cells_);
        o.w_ = 0;
        o.h_ = 0;
    }
    return *this;
}

int Field::W() const {
    return w_;
}

int Field::H() const {
    return h_;
}

bool Field::Inside(Coord p) const {
    return p.X() >= 0 && p.Y() >= 0 && p.X() < w_ && p.Y() < h_;
}

const Cell& Field::At(Coord p) const {
    return cells_[Idx(p.X(), p.Y())];
}

Cell& Field::At(Coord p) {
    return cells_[Idx(p.X(), p.Y())];
}

bool Field::IsPassable(Coord p) const {
    return Inside(p) && At(p).IsPassable();
}

void Field::Render(const Coord& player,
                   const std::vector<Coord>& enemies,
                   const std::vector<Coord>& allies,
                   const std::vector<Coord>& towers,
                   const std::vector<Coord>& turrets,
                   const std::vector<Coord>& traps) const
{
    std::vector<std::vector<char>> buf(h_, std::vector<char>(w_, '#'));

    for (int y = 0; y < h_; ++y) {
        for (int x = 0; x < w_; ++x) {
            auto t = cells_[Idx(x,y)].Type();
            char ch = '#';
            if (t == Cell::CellType::Floor) ch='_';
            else if (t == Cell::CellType::Slow) ch='~';
            buf[y][x] = ch;
        }
    }

    for (const auto& trap_pos : traps) {
        if (Inside(trap_pos)) {
            buf[trap_pos.Y()][trap_pos.X()] = '^';
        }
    }

    for (const auto& t : towers) {
        if (Inside(t)) {
            buf[t.Y()][t.X()] = 'T';
        }
    }

    for (const auto& r : turrets) {
        if (Inside(r)) {
            buf[r.Y()][r.X()] = 'R';
        }
    }

    for (const auto& e : enemies) {
        if (Inside(e)) {
            buf[e.Y()][e.X()] = 'E';
        }
    }

    for (const auto& a : allies) {
        if (Inside(a)) {
            buf[a.Y()][a.X()] = 'A';
        }
    }

    if (Inside(player)) {
        buf[player.Y()][player.X()] = 'P';
    }

    std::cout << "\n";
    for (int y = 0; y < h_; ++y) {
        for (int x = 0; x < w_; ++x) {
            std::cout << buf[y][x];
        }
        std::cout << "\n";
    }
}

Coord Field::RandomPassable(std::mt19937& rng) const {
    std::uniform_int_distribution<int> dx(0, w_-1);
    std::uniform_int_distribution<int> dy(0, h_-1);

    const int limit = std::min(w_ * h_, LIMIT);
    for (int tries = 0; tries < limit; ++tries) {
        Coord p(dx(rng), dy(rng));
        if (IsPassable(p)) {
            return p;
        }
    }

    for (int y = 0; y < h_; ++y) {
        for (int x = 0; x < w_; ++x) {
            Coord p(x,y);
            if (IsPassable(p)) {
                return p;
            }
        }
    }

    throw std::runtime_error("Field: no passable cells");
}

int Field::Idx(int x, int y) const {
    return y * w_ + x;
}

void Field::Generate(std::mt19937& rng) {
    std::fill(cells_.begin(), cells_.end(), Cell(Cell::CellType::Wall));

    const int miners = MINER_COUNT;
    Coord center(w_/2, h_/2);
    std::vector<Coord> miner_pos(miners, center);

    auto carve = [this](Coord p) {
        if (Inside(p)) {
            cells_[Idx(p.X(),p.Y())].SetType(Cell::CellType::Floor);
        }
    };

    int steps = (w_ * h_) * 3 / 2;
    std::uniform_int_distribution<int> dir(0,3);

    for (int s = 0; s < steps; ++s) {
        for (int i=0; i<miners; ++i) {
            carve(miner_pos[i]);

            int d = dir(rng);
            int nx = miner_pos[i].X() + (d==0) - (d==1); // 0:+x, 1:-x
            int ny = miner_pos[i].Y() + (d==2) - (d==3); // 2:+y, 3:-y
            Coord next_pos(nx, ny);

            if (Inside(next_pos)) {
                miner_pos[i] = next_pos;
            }
        }
    }

    carve(center);

    std::uniform_real_distribution<double> prob(0.0, 1.0);
    for (int y=0; y<h_; ++y) {
        for (int x=0; x<w_; ++x) {
            Coord p(x,y);
            if (cells_[Idx(x,y)].Type() == Cell::CellType::Floor) {
                if (prob(rng) < 0.10) {
                    cells_[Idx(x,y)].SetType(Cell::CellType::Slow);
                }
            }
        }
    }
}
