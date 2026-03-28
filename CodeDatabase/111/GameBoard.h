#pragma once
#include "Location.h"
#include <vector>
#include <random>

class GameActor;

class GameBoard {
private:
    size_t width;
    size_t height;
    std::vector<Location> obstacles;
    std::vector<GameActor*> actors;

public:
    GameBoard(size_t width, size_t height);
    GameBoard(size_t width, size_t height, const std::vector<Location>& obstacles);
    
    GameBoard(const GameBoard& other);
    GameBoard(GameBoard&& other) noexcept;
    GameBoard& operator=(const GameBoard& other);
    GameBoard& operator=(GameBoard&& other) noexcept;
    ~GameBoard();
    
    bool isLocationValid(const Location& loc) const;
    bool isLocationFree(const Location& loc) const;
    bool isObstacle(const Location& loc) const;
    bool isMoveValid(const Location& from, const Location& to, GameActor* actor) const;
    
    void addActor(GameActor* actor);
    void moveActor(const Location& from, const Location& to, GameActor* actor);
    GameActor* getActorAt(const Location& loc) const;
    void removeAllActors();
    
    size_t getWidth() const;
    size_t getHeight() const;
    int calculateDistance(const Location& a, const Location& b) const;
    
    void generateRandomObstacles(int minCount, int maxCount);

private:
    void copyFrom(const GameBoard& other);
    void clear();
};