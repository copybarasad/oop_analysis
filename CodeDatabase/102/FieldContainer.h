#pragma once
#include <string>
#include <vector>
#include "../Units.h"
#include "../Entities/PositionEntity.h"

class FieldContainer
{
    std::vector<Cell> _grid;
    std::vector<Cell*> _wallGrid;
    std::vector<PositionEntity*> _entities;
    int _width;
    int _height;
public:
    struct FieldData
    {
        int _fieldWidth;
        int _fieldHeight;
        std::vector<Cell> _walls;

        explicit FieldData(const FieldContainer* container);

        explicit FieldData(std::string& s);

        std::string ToString() const;
    };

    FieldContainer(int width, int height);

    FieldContainer(int width, int height, std::vector<Cell>& walls);

    FieldContainer(const FieldContainer& other);

    FieldContainer(FieldContainer&& other) noexcept;

    FieldContainer& operator=(const FieldContainer& other);

    FieldContainer& operator=(FieldContainer&& other) noexcept;

    bool AddEntity(PositionEntity* entity);

    bool RemoveEntity(PositionEntity* entity);

    void RemoveEntities();

    void RemoveCell(Cell cell);

    void RemoveCell(int x, int y);

    PositionEntity* GetEntity(Cell cell) const;

    Cell GetCell(int x, int y) const;

    std::vector<Cell*> GetWalls() const;

    int GetDistance(const Cell a, const Cell b) const;

    int GetWidth() const;

    int GetHeight() const;
private:
    void CopyFrom(const FieldContainer& other);
    
    void Clear();
};