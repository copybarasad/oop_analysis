#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H

#include "Game.h"
#include <string>

class SaveManager {
public:
    static void saveGameToFile(const Game& game, const std::string& filename);
    static void loadGameFromFile(Game& game, const std::string& filename);
};

#endif // SAVEMANAGER_H
