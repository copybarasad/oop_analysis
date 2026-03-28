#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H

#include "GameEngine.h"
#include "GameException.h"
#include "SaveGameException.h"
#include "LoadGameException.h"
#include "FileNotFoundException.h"
#include "CorruptedSaveException.h"
#include <string>
#include <fstream>

class GameEngine;
class GameField;

class SaveManager {
public:
    void saveGame(const std::string& filename, const GameEngine* gameEngine, int currentLevel);
    void loadGame(const std::string& filename, GameEngine* gameEngine, int& currentLevel);
    void saveGameState(std::ofstream& file, const GameEngine* gameEngine);
    void loadGameState(std::ifstream& file, GameEngine* gameEngine);
    void saveFieldState(std::ofstream& file, const GameField* field);
    void loadFieldState(std::ifstream& file, GameField* field);
};

#endif