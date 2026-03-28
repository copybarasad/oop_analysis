#pragma once
#include <vector>
#include <string>
#include "node.h"

class Player;
class Enemy;

class Field {
private:
    int width;
    int length;
    std::vector<std::vector<Node>> arr;
    
    void createObstacleField(int obsPerc);
    std::vector<std::pair<int, int>> obsFromUser();
    void createObstacleField(const std::vector<std::pair<int, int>>& coords);
    void setlength(int y);
    void setwidth(int x);
    void checkObs(int obs);
    void resizeInternal(int newWidth, int newHeight);
public:
    Field(int x, int y);
    Field(int x, int y, bool fromUser, int obsPerc = 0);
    Field(const Field& other);
    Field(Field&& other) noexcept;
    Field& operator=(const Field& other);
    Field& operator=(Field&& other) noexcept;
    ~Field() = default;

    bool validpos(int x, int y) const;
    int getwidth() const;
    int getlength() const;
    Node& getNode(int x, int y);
    const Node& getNode(int x, int y) const;
    
    bool placePlayer(int x, int y);
    bool placeEnemy(int x, int y); 
    bool placeTower(int x, int y);

    void resize(int newWidth, int newHeight);
    void resizeWithObstacles(int newWidth, int newHeight, int obstaclePercent);
};