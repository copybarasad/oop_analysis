#ifndef GAME_FIELD_H
#define GAME_FIELD_H

#include <vector>

class Player;
class Enemy;

class GameField {
private:
    int fieldWidth;
    int fieldHeight;
    std::vector<std::vector<bool>> obstacleGrid;

    void deepCopyFrom(const GameField& other);

public:
    GameField(int width = 15, int height = 15);
    GameField(const GameField& other);
    GameField(GameField&& other) noexcept;
    GameField& operator=(const GameField& other);
    GameField& operator=(GameField&& other) noexcept;
    ~GameField() = default;

    int getFieldWidth() const;
    int getFieldHeight() const;
    bool isPositionWithinBounds(int x, int y) const;
    bool isCellPassable(int x, int y) const;
    
    bool isObstacle(int x, int y) const;
    void setObstacle(int x, int y, bool blocked);
    
    void createRandomObstacles();

    char getCellRepresentation(int x, int y, const Player& player, const std::vector<Enemy>& enemies) const;
};

#endif