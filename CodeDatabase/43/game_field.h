    #ifndef GAME_FIELD_H
    #define GAME_FIELD_H

    #include <vector>
    #include <memory>
    #include "entity.h"

    enum class CellType {
        Empty,
        Wall
    };

    class GameField {
    private:
        size_t width, height;
        std::vector<std::vector<CellType>> grid;
        std::vector<std::shared_ptr<Entity>> entities;
        std::vector<std::pair<int, int>> entityPositions;

    public:
        GameField(size_t w, size_t h);

        GameField(const GameField& other);
        GameField(GameField&& other) noexcept;

        GameField& operator=(const GameField& other);
        GameField& operator=(GameField&& other) noexcept;

        size_t getWidth() const;
        size_t getHeight() const;
        bool isWithinBounds(int x, int y) const;
        bool isCellPassable(int x, int y) const;
        CellType getCellType(int x, int y) const;

        bool hasClearLineOfSight(int x1, int y1, int x2, int y2) const;
        std::pair<int, int> getRandomEmptyCell() const;

        void addEntity(std::shared_ptr<Entity> entity, int x, int y);
        void removeEntity(int x, int y);
        std::shared_ptr<Entity> getEntityAt(int x, int y) const;
        const std::vector<std::shared_ptr<Entity>>& getEntities() const;
        bool moveEntity(int fromX, int fromY, int toX, int toY);

        char getCellSymbol(int x, int y) const;
        void display() const;

        void setCellType(int x, int y, CellType type);

    private:
        void initializeWalls();
        void deepCopy(const GameField& other);
    };

    #endif