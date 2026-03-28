#ifndef GAME_LOADER_H
#define GAME_LOADER_H

#include <string>

class Game;

class GameLoader {
public:
    void load(const std::string &filename, Game &game);
};

#endif
