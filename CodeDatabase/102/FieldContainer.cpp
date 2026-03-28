#include "../../Headers/Containers/FieldContainer.h"
#include <cmath>
#include <sstream>

FieldContainer::FieldData::FieldData(const FieldContainer* container):
    _fieldWidth{ container->_width }, _fieldHeight{ container->_height }
{
    for (Cell* wall : container->_wallGrid)
        _walls.push_back(*wall);
}

FieldContainer::FieldData::FieldData(std::string& s)
{
    std::istringstream stream(s);
    std::string line;
    while (line != "FIELD_S" && std::getline(stream, line))
    {
    }
    if (line.empty())
        return;
    std::getline(stream, line);
    _fieldWidth = std::stoi(line);
    std::getline(stream, line);
    _fieldHeight = std::stoi(line);
    while (std::getline(stream, line) && line != "FIELD_E")
    {
        std::istringstream wallStream(line);
        int x, y;
        wallStream >> x >> y;
        _walls.emplace_back(x, y);
    }
}

std::string FieldContainer::FieldData::ToString() const
{
    std::string s = "FIELD_S\n" + std::to_string(_fieldWidth) + '\n' + std::to_string(_fieldHeight) + '\n';
    for (Cell wall : _walls)
        s += std::to_string(wall._x) + ' ' + std::to_string(wall._y) + '\n';
    s += "FIELD_E";
    return s;
}

FieldContainer::FieldContainer(int width, int height) : _width(width), _height(height)
{
    for (int i = 0; i < width; i++)
    {
        for (int i1 = 0; i1 < height; i1++)
            _grid.emplace_back(i, i1);
    }
}

FieldContainer::FieldContainer(int width, int height, std::vector<Cell>& walls) : FieldContainer(width, height)
{
    for (Cell cell : walls)
        RemoveCell(cell);
}

FieldContainer::FieldContainer(const FieldContainer& other)
    : _width(other._width), _height(other._height)
{
    CopyFrom(other);
}

FieldContainer::FieldContainer(FieldContainer&& other) noexcept 
    : _width(other._width), _height(other._height)
{
    _grid = std::move(other._grid);
    _entities = std::move(other._entities);
    _wallGrid.reserve(other._wallGrid.size());
    for (Cell* wall : other._wallGrid)
        RemoveCell(*wall);
    other._width = 0;
    other._height = 0;
    other._wallGrid.clear();
}

FieldContainer& FieldContainer::operator=(const FieldContainer& other) 
{
    if (this != &other) 
    {
        Clear();
        _width = other._width;
        _height = other._height;
        CopyFrom(other);
    }
    return *this;
}

FieldContainer& FieldContainer::operator=(FieldContainer&& other) noexcept 
{
    if (this != &other) 
    {
        Clear();
        _width = other._width;
        _height = other._height;
        _grid = std::move(other._grid);
        _entities = std::move(other._entities);
        _wallGrid = std::move(other._wallGrid);
        other._width = 0;
        other._height = 0;
    }
    return *this;
}

void FieldContainer::CopyFrom(const FieldContainer& other) 
{
    _grid = other._grid;
    _wallGrid.reserve(other._wallGrid.size());
    for (Cell* wall : other._wallGrid)
        RemoveCell(*wall);
    _entities = other._entities;
}

void FieldContainer::Clear() 
{
    _wallGrid.clear();
    _grid.clear();
    _entities.clear();
    _width = 0;
    _height = 0;
}

bool FieldContainer::AddEntity(PositionEntity* entity)
{
    if (GetEntity(entity->GetPosition()) != nullptr)
        return false;
    for (PositionEntity* currentEntity : _entities)
    {
        if (currentEntity == entity)
            return false;
    }
    _entities.push_back(entity);
    return true;
}

bool FieldContainer::RemoveEntity(PositionEntity* entity)
{
    for (int i = 0; i < _entities.size(); i++)
    {
        if (_entities[i] == entity)
        {
            _entities.erase(_entities.begin() + i);
            return true;
        }
    }
    return false;
}

void FieldContainer::RemoveEntities()
{
    _entities.clear();
}

void FieldContainer::RemoveCell(Cell cell)
{
    _wallGrid.push_back(&_grid[cell._x * _height + cell._y]);
}

void FieldContainer::RemoveCell(int x, int y)
{
    _wallGrid.push_back(&_grid[x * _height + y]);
}

PositionEntity* FieldContainer::GetEntity(Cell cell) const
{
    for (auto currEntity : _entities)
    {
        Cell position = currEntity->GetPosition();
        if (position == cell)
            return currEntity;
    }
    return nullptr;
}

Cell FieldContainer::GetCell(int x, int y) const
{
    if (x >= _width || y >= _height || x < 0 || y < 0)
        return Cell();
    Cell cell = _grid[x * _height + y];
    for (Cell* wall : _wallGrid)
    {
        if (*wall == cell)
            return Cell();
    }
    return cell;
}

std::vector<Cell *> FieldContainer::GetWalls() const
{
    return _wallGrid;
}

int FieldContainer::GetDistance(const Cell a, const Cell b) const
{
    return abs(a._x - b._x) + abs(a._y - b._y);
}

int FieldContainer::GetWidth() const
{
    return _width;
}

int FieldContainer::GetHeight() const
{
    return _height;
}