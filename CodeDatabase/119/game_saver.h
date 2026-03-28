#ifndef GAME_SAVER_H
#define GAME_SAVER_H

#include <string>

class Game;

class GameSaver {
public:
    void save(const std::string &filename, const Game &game);
};

#endif
