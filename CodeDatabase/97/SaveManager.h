#ifndef UNTITLED_SAVEMANAGER_H
#define UNTITLED_SAVEMANAGER_H

#include <string>

class GameState;

namespace SaveManager {
    void save(const GameState &state, const std::string &filename);
    void load(GameState &state, const std::string &filename);
}

#endif //UNTITLED_SAVEMANAGER_H
