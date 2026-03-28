#include "GameField.h"

GameField::GameField(std::pair<unsigned int,unsigned int> size, float wallChance, float slowChance) :
        width(std::clamp<unsigned int>(size.first, 10, 25)), height(std::clamp<unsigned int>(size.second, 10, 25)),
        wallChance(std::clamp(wallChance, 0.0f, 0.4f)), slowChance(std::clamp(slowChance, 0.0f, 0.5f)),
        gen(std::random_device{}()) {
    generate();
}

GameField::GameField(json& data) : gen(std::random_device{}()) {
    if (!data.contains("width"))
        throw std::runtime_error("Отсутствует поле width у игрового поля");
    if (!data["width"].is_number_unsigned())
        throw std::runtime_error("Значение поля width у игрового поля не является беззнаковым числом");
    width = data["width"].get<unsigned int>();
    if (!(10 <= width && width <= 25))
        throw std::runtime_error("Значение поля width у игрового поля принимает некорректное значение");

    if (!data.contains("height"))
        throw std::runtime_error("Отсутствует поле height у игрового поля");
    if (!data["height"].is_number_unsigned())
        throw std::runtime_error("Значение поля height у игрового поля не является беззнаковым числом");
    height = data["height"].get<unsigned int>();
    if (!(10 <= height && height <= 25))
        throw std::runtime_error("Значение поля height у игрового поля принимает некорректное значение");

    if (!data.contains("wallChance"))
        throw std::runtime_error("Отсутствует поле wallChance у игрового поля");
    if (!data["wallChance"].is_number_float())
        throw std::runtime_error("Значение поля wallChance у игрового поля не является вещественным числом");
    wallChance = data["wallChance"].get<float>();
    if (!(0.0f <= wallChance && wallChance <= 0.4f))
        throw std::runtime_error("Значение поля slowChance у игрового поля  принимает неправильное значение");

    if (!data.contains("slowChance"))
        throw std::runtime_error("Отсутствует поле slowChance у игрового поля");
    if (!data["slowChance"].is_number_float())
        throw std::runtime_error("Значение поля slowChance у игрового поля не является вещественным числом");
    slowChance = data["slowChance"].get<float>();
    if (!(0.0f <= slowChance && slowChance <= 0.5f))
        throw std::runtime_error("Значение поля slowChance у игрового поля  принимает некорректное значение");

    if (!data.contains("cells"))
        throw std::runtime_error("Отсутствует поле cells у игрового поля");
    if (!data["cells"].is_array())
        throw std::runtime_error("Значение поля cells у игрового поля не является массивом");
    if (data["cells"].size() != width * height)
        throw std::runtime_error("Количество сохранённых клеток не совпадает с размерами поля");
    cells = std::vector(height, std::vector<Cell>(width));
    for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x)
            cells[y][x] = Cell(data["cells"][y * width + x]);
}

json GameField::to_json() const {
    json j;
    j["width"] = width;
    j["height"] = height;
    j["wallChance"] = wallChance;
    j["slowChance"] = slowChance;
    std::vector<json> cells_json;
    cells_json.reserve(width * height);
    for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x)
            cells_json.push_back(cells[y][x].to_json());
    j["cells"] = cells_json;
    return j;
}

void GameField::uniteSeparatedAreas() {
    std::vector<std::vector<int>> area(height, std::vector<int>(width, -1));
    std::map<int, std::vector<std::pair<int,int>>> areas;
    int areaCount = 0;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (cells[y][x].getPassability() == CellType::WALL || area[y][x] != -1)
                continue;
            std::queue<std::pair<int,int>> q;
            q.emplace(x, y);
            area[y][x] = areaCount;
            int areaMembersCount = 1;
            std::pair<int,int> directions[] = {{0,1}, {1,0}, {-1,0}, {0,-1}};
            while (!q.empty()) {
                auto [cx, cy] = q.front();
                q.pop();
                areas[areaCount].emplace_back(cx, cy);
                for (auto direction : directions) {
                    int nx = cx + direction.first;
                    int ny = cy + direction.second;
                    if (nx < 0 || nx >= width || ny < 0 || ny >= height)
                        continue;
                    if (cells[ny][nx].getPassability() == CellType::WALL)
                        continue;
                    if (area[ny][nx] != -1)
                        continue;
                    area[ny][nx] = areaCount;
                    areaMembersCount++;
                    q.emplace(nx, ny);
                }
            } if (areaMembersCount == 1) {
                cells[y][x].setPassability(CellType::WALL); areas.erase(areaCount);
            } else {
                areaCount++;
            }
        }
    } if (areaCount <= 1) return;

    double bestDist = 1e9;
    std::pair<int,int> bestWall = {-1, -1};

    for (int y = 1; y < height - 1; ++y) {
        for (int x = 1; x < width - 1; ++x) {
            if (cells[y][x].getPassability() != CellType::WALL) continue;

            std::set<int> neighborAreas;
            for (auto [dx, dy] : std::array<std::pair<int,int>,4>{{{1,0},{-1,0},{0,1},{0,-1}}}) {
                int nx = x + dx, ny = y + dy;
                if (!isInBounds({nx, ny})) continue;

                for (int i = 0; i < areaCount; ++i) {
                    if (std::ranges::find(areas[i].begin(), areas[i].end(), std::pair<int,int>{nx, ny}) != areas[i].end()) {
                        neighborAreas.insert(i);
                    }
                }
            }

            if (neighborAreas.size() >= 2) {
                double dist = std::abs(x - width / 2.0) + std::abs(y - height / 2.0);
                if (dist < bestDist) {
                    bestDist = dist;
                    bestWall = {x, y};
                }
            }
        }
    }

    if (bestWall.first != -1 && bestWall.second != -1) {
        if (isInBounds(bestWall))
            cells[bestWall.second][bestWall.first].setPassability(CellType::CLEAR);
        uniteSeparatedAreas();
        return;
    }
    auto [x1, y1] = areas[0][areas[0].size() / 2];
    auto [x2, y2] = areas[1][areas[1].size() / 2];
    for (int x = std::min(x1, x2); x <= std::max(x1, x2); ++x)
        if (isInBounds({x, y1}))
            cells[y1][x].setPassability(CellType::CLEAR);
    for (int y = std::min(y1, y2); y <= std::max(y1, y2); ++y)
        if (isInBounds({x2, y}))
            cells[y][x2].setPassability(CellType::CLEAR);
    uniteSeparatedAreas();
}

GameField::GameField(const GameField& other) : width(other.width), height(other.height), wallChance(other.wallChance),
                                               slowChance(other.slowChance), cells(other.cells), gen(other.gen) {}

GameField& GameField::operator=(const GameField& other) {
    if (this == &other) return *this;
    width = other.width;
    height = other.height;
    wallChance = other.wallChance;
    slowChance = other.slowChance;
    cells = other.cells;
    gen = other.gen;
    return *this;
}

GameField::GameField(GameField&& other) noexcept
    : width(other.width), height(other.height), wallChance(other.wallChance), slowChance(other.slowChance),
        cells(std::move(other.cells)), gen(other.gen) {}

GameField& GameField::operator=(GameField&& other) noexcept {
    if (this == &other) return *this;

    width = other.width;
    height = other.height;
    wallChance = other.wallChance;
    slowChance = other.slowChance;
    cells = std::move(other.cells);
    gen = other.gen;

    return *this;
}

std::vector<std::pair<int,int>> GameField::getPassableCells() const {
    std::vector<std::pair<int,int>> result;
    for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x)
            if (!cells[y][x].isOccupied() && cells[y][x].getPassability() != CellType::WALL)
                result.emplace_back(x, y);
    return result;
}

Cell& GameField::getCell(const std::pair<int,int>& coordinates) {
    return cells[coordinates.second][coordinates.first];
}

std::pair<int,int> GameField::getFreeCellCoordinates() {
    auto passableCells = getPassableCells();
    std::ranges::shuffle(passableCells.begin(), passableCells.end(), gen);
    if (passableCells.empty())
        return {-1, -1};
    return passableCells.back();
}

bool GameField::isInBounds(const std::pair<int,int>& coordinates) const {
    return coordinates.first >= 0 && coordinates.first < width &&
           coordinates.second >= 0 && coordinates.second < height;
}

bool GameField::areCoordinatesInArea(const std::pair<int, int>& coordinates, const std::pair<int, int>& area, const int& areaDiameter) const {
    auto [x, y] = coordinates;
    auto [ax, ay] = area;
    if (!isInBounds(coordinates)) return false;
    return ((0 <= x - ax && x - ax <= areaDiameter / 2) || (0 <= ax - x && ax - x <= areaDiameter / 2 - !(areaDiameter % 2))) &&
            ((0 <= y - ay && y - ay <= areaDiameter / 2) || (0 <= ay - y && ay - y <= areaDiameter / 2 - !(areaDiameter % 2)));
}

int GameField::getHeight() const { return height; }

int GameField::getWidth() const { return width; }

std::mt19937& GameField::getRandomGenerator() { return gen; }

int GameField::collectPoints() {
    int points = 0;
    for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x)
            points += cells[y][x].collectPoints();
    return points;
}

void GameField::expand() {
    width = std::clamp<unsigned int>(width + 1, 10, 25);
    height = std::clamp<unsigned int>(height + 1, 10, 25);
    generate();
}

void GameField::generate() {
    cells.clear();
    cells = std::vector(height, std::vector<Cell>(width));
    std::uniform_real_distribution<> dist(0.0, 1.0);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            double r = dist(gen);
            if (r < wallChance)
                cells[y][x].setPassability(CellType::WALL);
            else if (r < wallChance + slowChance)
                cells[y][x].setPassability(CellType::SLOW);
        }
    }
    uniteSeparatedAreas();
}
