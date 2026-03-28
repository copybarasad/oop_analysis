#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H

#include <cstddef>
class Game;

class LevelManager {
public:
    
    static void advanceToNextLevel(Game& game);
};

#endif