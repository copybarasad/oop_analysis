#pragma once
#include <string>
#include "Game.h"

class SaveManager {
public:
    static void saveGame(const Game& game, const std::string& filename = "savegame.dat");
    static void loadGame(Game& game, const std::string& filename = "savegame.dat");
    static bool saveFileExists(const std::string& filename = "savegame.dat");
};