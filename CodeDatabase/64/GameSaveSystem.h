#ifndef GAMESAVESYSTEM_H
#define GAMESAVESYSTEM_H

#include "Game.h"
#include <fstream>
#include <vector>
#include <string>

class GameSaveSystem {
public:
    static bool saveGame(const Game& game, const std::string& filename);
    static bool loadGame(Game& game, const std::string& filename);
    static std::vector<std::string> getSaveFiles();
    static bool saveExists(const std::string& filename);

private:
    static void writeInt(std::ofstream& file, int value);
    static int readInt(std::ifstream& file);
    static void writeBool(std::ofstream& file, bool value);
    static bool readBool(std::ifstream& file);
    static void writeString(std::ofstream& file, const std::string& str);
    static std::string readString(std::ifstream& file);
};

#endif