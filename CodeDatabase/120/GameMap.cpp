#include "GameMap.h"
GameMap::GameMap(int w, int h)
    : width(w < 10 ? 10 : (w > 25 ? 25 : w)),
      height(h < 10 ? 10 : (h > 25 ? 25 : h)) {
  std::srand((std::time(nullptr)));

  cells.resize(h, std::vector<Cell>(w, Cell(Cell::CellType::Empty)));

  for (int y = 0; y < h; ++y) {
    for (int x = 0; x < w; ++x) {
      int r = std::rand() % 100;
      if (r < 5)
        cells[y][x].SetType(Cell::CellType::Wall);
      else if (r < 15)
        cells[y][x].SetType(Cell::CellType::Slow);
      else
        cells[y][x].SetType(Cell::CellType::Empty);
    }
  }
}

GameMap::GameMap(const GameMap& other)
    : width(other.width), height(other.height), cells(other.cells) {}

GameMap::GameMap(GameMap&& other)
    : width(other.width), height(other.height), cells(std::move(other.cells)) {
  other.width = 0;
  other.height = 0;
}

GameMap& GameMap::operator=(const GameMap& other) {
  if (this != &other) {
    width = other.width;
    height = other.height;
    cells = other.cells;
  }
  return *this;
}
int GameMap::GetWidth() const { return width; }
int GameMap::GetHeight() const { return height; }
GameMap& GameMap::operator=(GameMap&& other) {
  if (this != &other) {
    width = other.width;
    height = other.height;
    cells = std::move(other.cells);

    other.width = 0;
    other.height = 0;
  }
  return *this;
}

bool GameMap::IsInside(int x, int y) const {
  return x >= 0 && y >= 0 && x < width && y < height;
}

bool GameMap::IsWalkable(int x, int y) const {
  if (!IsInside(x, y)) return false;
  return cells[y][x].IsWalkable();
}

Cell::CellType GameMap::GetCellType(int x, int y) const {
  return cells[y][x].GetType();
}

std::optional<Position> GameMap::FindRandomFreeCell() const {
  std::vector<Position> freeCells;
  for (int y = 0; y < height; ++y)
    for (int x = 0; x < width; ++x)
      if (cells[y][x].IsWalkable()) freeCells.push_back({x, y});

  if (freeCells.empty()) return std::nullopt;

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dist(0, (freeCells.size()) - 1);
  return freeCells[dist(gen)];
}
void GameMap::SetCells(std::vector<std::vector<Cell>>&& newCells) {
  cells = std::move(newCells);
  height = static_cast<int>(cells.size());
  width = height > 0 ? static_cast<int>(cells[0].size()) : 0;
}

std::vector<Position> GameMap::FindFreeAdjacent(const Position& pos) const {
  const int DX[4] = {1, -1, 0, 0};
  const int DY[4] = {0, 0, 1, -1};
  std::vector<Position> adj;

  for (int i = 0; i < 4; ++i) {
    int nx = pos.x + DX[i];
    int ny = pos.y + DY[i];
    if (GameMap::IsInside(nx, ny) && GameMap::IsWalkable(nx, ny)) {
      adj.push_back(Position{nx, ny});
    }
  }
  return adj;
}
