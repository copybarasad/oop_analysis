#ifndef SAVE_MANAGER_H
#define SAVE_MANAGER_H

#include <string>

class Game;

class SaveManager {
public:
    static void saveGameToFile(const Game& game, const std::string& filename);
    static void loadGameFromFile(Game& game, const std::string& filename);
};

#endif