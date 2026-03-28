#include <cstddef>
#include <random>
#include <stdexcept>
#include <vector>

#include "../field/generic-field.hpp"
#include "../field/slime-field.hpp"
#include "../field/wall-field.hpp"
#include "euclidean-map.hpp"

EuclideanMap::EuclideanMap(int width, int height) {
    if (width < 10 || width > 25) {
        throw std::invalid_argument("width must be in range [10, 25]");
    }

    if (height < 10 || height > 25) {
        throw std::invalid_argument("height must be in range [10, 25]");
    }

    m_Width = width;
    m_Height = height;

    for (int y = 0; y < height; y++) {
        std::vector<Field *> row;

        for (int x = 0; x < width; x++) {
            Field *field = randomField({x, y});
            row.push_back(field);
        }

        m_Fields.push_back(row);
    }
}

EuclideanMap::EuclideanMap(const EuclideanMap &other) {
    m_Width = other.m_Width;
    m_Height = other.m_Height;

    for (int y = 0; y < m_Height; y++) {
        std::vector<Field *> row;

        for (int x = 0; x < m_Width; x++) {
            row.push_back(other.m_Fields[y][x]->clone());
        }

        m_Fields.push_back(row);
    }
}

EuclideanMap::EuclideanMap(EuclideanMap &&other)
    : m_Fields(std::move(other.m_Fields)) {
    m_Width = other.m_Width;
    m_Height = other.m_Height;
}

EuclideanMap::~EuclideanMap() {
    for (int y = 0; y < m_Height; y++) {
        for (int x = 0; x < m_Width; x++) {
            delete m_Fields[y][x];
        }

        m_Fields[y].clear();
    }

    m_Fields.clear();
}

Map *EuclideanMap::clone() { return new EuclideanMap(*this); }

EuclideanMap &EuclideanMap::operator=(const EuclideanMap &other) {
    if (this == &other)
        return *this;

    m_Width = other.m_Width;
    m_Height = other.m_Height;

    for (int y = 0; y < m_Height; y++) {
        std::vector<Field *> row;

        for (int x = 0; x < m_Width; x++) {
            row.push_back(other.m_Fields[y][x]->clone());
        }

        m_Fields.push_back(row);
    }

    return *this;
}

EuclideanMap &EuclideanMap::operator=(EuclideanMap &&other) {
    if (this == &other)
        return *this;

    m_Fields = std::move(other.m_Fields);
    m_Width = other.m_Width;
    m_Height = other.m_Height;

    return *this;
}

Field *EuclideanMap::at(Position position) {
    if (outOfBounds(position)) {
        return nullptr;
    }

    return m_Fields[position.y][position.x];
}

Field *EuclideanMap::randomNearby(Position position) {
    std::vector<Position> nearbyFields;

    int x = position.x;
    int y = position.y;

    Position positions[] = {
        {x - 1, y - 1}, {x, y - 1},     {x + 1, y - 1}, {x - 1, y},
        {x + 1, y},     {x - 1, y + 1}, {x, y + 1},     {x + 1, y + 1},
    };

    for (auto pos : positions) {
        if (outOfBounds(pos)) {
            continue;
        }

        if (at(pos) != nullptr) {
            nearbyFields.push_back(pos);
        }
    }

    int index = rand() % nearbyFields.size();
    Position resultPosition = nearbyFields[index];

    nearbyFields.clear();
    nearbyFields.shrink_to_fit();

    return at(resultPosition);
}

bool EuclideanMap::outOfBounds(Position position) {
    if (position.x < 0 || position.x >= m_Width) {
        return true;
    }

    if (position.y < 0 || position.y >= m_Height) {
        return true;
    }

    return false;
}

Field *EuclideanMap::fieldInDirection(Position position, Direction direction) {
    std::pair<int, int> moveVector = directionToVec2D(direction);

    int dx = moveVector.first;
    int dy = moveVector.second;

    return at({position.x + dx, position.y + dy});
}

Field *EuclideanMap::randomField(Position pos) {
    bool isRim = pos.x == 0 || pos.x == m_Width - 1 || pos.y == 0 ||
                 pos.y == m_Height - 1;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 100);

    int choice = dis(gen);

    if (choice >= 95 && !isRim) {
        return new WallField();
    }

    if (choice >= 85) {
        return new SlimeField();
    }

    return new GenericField();
}

std::pair<int, int> EuclideanMap::directionToVec2D(Direction direction) {
    switch (direction) {
    case Direction::North:
        return {0, -1};
    case Direction::NorthEast:
        return {1, -1};
    case Direction::East:
        return {1, 0};
    case Direction::SouthEast:
        return {1, 1};
    case Direction::South:
        return {0, 1};
    case Direction::SouthWest:
        return {-1, 1};
    case Direction::West:
        return {-1, 0};
    case Direction::NorthWest:
        return {-1, -1};
    default:
        return {0, 0};
    }
}

Position EuclideanMap::locateField(Field *field) {
    for (int y = 0; y < m_Height; y++) {
        for (int x = 0; x < m_Width; x++) {
            if (m_Fields[y][x] == field) {
                return Position{x, y};
            }
        }
    }

    throw std::invalid_argument("map does not contain field");
}

double EuclideanMap::distance(Position p1, Position p2) {
    double dx = (double)(p1.x - p2.x);
    double dy = (double)(p1.y - p2.y);

    return std::sqrt(dx * dx + dy * dy);
}

bool EuclideanMap::canSpawnAt(Position where) {
    return !outOfBounds(where) && at(where)->canOccupy();
}

void EuclideanMap::spawn(Creature *creture, Position where) {
    if (outOfBounds(where)) {
        throw std::range_error("position is out of bounds");
    }

    if (!at(where)->canOccupy()) {
        throw std::logic_error("field at position is occupied");
    }

    trySpawnAlly(creture, where) || trySpawnEnemy(creture, where);
}

bool EuclideanMap::trySpawnAlly(Creature *creture, Position where) {
    Ally *ally = dynamic_cast<Ally *>(creture);
    if (ally == nullptr) {
        return false;
    }

    ally->setMap(this);
    ally->setPosition(where);
    at(where)->occupy(ally);
    m_SpawnedAllies.push_back(ally);
    return true;
}

bool EuclideanMap::trySpawnEnemy(Creature *creture, Position where) {
    Enemy *enemy = dynamic_cast<Enemy *>(creture);
    if (enemy == nullptr) {
        return false;
    }

    enemy->setMap(this);
    enemy->setPosition(where);
    at(where)->occupy(enemy);
    m_SpawnedEnemies.push_back(enemy);
    return true;
}

std::vector<Ally *> EuclideanMap::extractSpawnedAllies() {
    std::vector<Ally *> result = std::move(m_SpawnedAllies);
    m_SpawnedAllies.clear();
    return result;
}

std::vector<Enemy *> EuclideanMap::extractSpawnedEnemies() {
    std::vector<Enemy *> result = std::move(m_SpawnedEnemies);
    m_SpawnedEnemies.clear();
    return result;
}

bool EuclideanMap::winConditionIsMet() {
    for (int y = 0; y < m_Height; y++) {
        for (int x = 0; x < m_Width; x++) {
            if (m_Fields[y][x]->isOccupiedByEnemy()) {
                return true;
            }
        }
    }

    return false;
}

std::string EuclideanMap::toString() {
    // color + char + reset + LF.
    const size_t fieldLength = 5 + 1 + 4 + 1;
    std::string result;
    result.reserve(m_Width * m_Height * fieldLength);

    for (int y = 0; y < m_Height; y++) {
        for (int x = 0; x < m_Width; x++) {
            Field *field = at({x, y});

            result += field->color().ansiEscapeCode();
            result += field->chr();
            result += Color::reset().ansiEscapeCode();
        }
        result += '\n';
    }

    return result;
}

void EuclideanMap::update() {
    for (int y = 0; y < m_Height; y++) {
        for (int x = 0; x < m_Width; x++) {
            at({x, y})->update();
        }
    }
}

bytedump EuclideanMap::dump() {
    bytedump bytes;
    bytes.reserve(m_Width * m_Height + 2);

    bytes.push_back(static_cast<std::byte>(m_Height));
    bytes.push_back(static_cast<std::byte>(m_Width));

    for (int y = 0; y < m_Height; y++) {
        for (int x = 0; x < m_Width; x++) {
            std::byte fieldByte = at({x, y})->dump()[0];
            bytes.push_back(fieldByte);
        }
    }

    return bytes;
}

EuclideanMap::EuclideanMap(bytedump bytes) {
    if (bytes.size() < 2) {
        throw std::length_error("map dump must contain at least 2 bytes");
    }

    m_Height = static_cast<int>(bytes[0]);
    m_Width = static_cast<int>(bytes[1]);

    if (bytes.size() < 2 + m_Height * m_Width) {
        throw std::length_error("map dump size mismatch");
    }

    for (int y = 0; y < m_Height; y++) {
        std::vector<Field *> row;

        for (int x = 0; x < m_Width; x++) {
            std::byte b = bytes[2 + y * m_Height + x];
            row.push_back(Field::fromByte(b));
        }

        m_Fields.push_back(row);
    }
}
