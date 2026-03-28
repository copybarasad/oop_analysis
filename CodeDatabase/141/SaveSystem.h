#ifndef SAVE_SYSTEM_H
#define SAVE_SYSTEM_H

#include <string>
#include <fstream>
#include <vector>

class Game;
struct GameSnapshot;

class SaveSystem {
public:
    static void saveGame(const Game &game, const std::string &filename);

    static void loadGame(Game &game, const std::string &filename);

    static bool saveFileExists(const std::string &filename);

    static std::vector<std::string> listSaveSlots();

    static std::string getSaveFilePath(const std::string &slotName);

private:
    static void writeGameState(std::ofstream &file, const GameSnapshot &snapshot);

    static GameSnapshot readGameState(std::ifstream &file);

    static void validateFileHeader(std::ifstream &file);

    static void writeFileHeader(std::ofstream &file);
};

#endif
