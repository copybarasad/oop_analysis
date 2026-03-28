#include <vector>
#include <iostream>
#include "LevelManager/LevelManager.hpp"

class ConsoleRenderer {
public:
    void draw(GameDTO& dto);
    bool isAlive();
};

