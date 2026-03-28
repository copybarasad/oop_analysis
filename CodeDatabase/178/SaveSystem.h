#ifndef SAVE_SYSTEM_H
#define SAVE_SYSTEM_H

#include "GameController.h"
#include "LevelManager.h"
#include "Exceptions.h"
#include "GameSaveData.h"
#include "SpellSaveData.h"
#include <string>
#include <memory>
#include <vector>

class SaveSystem {
private:
    static const std::string SAVE_FILE_PATH;

public:
    static void saveGame(const GameController& controller, const LevelManager& levelManager);
    static std::pair<std::unique_ptr<GameController>, std::unique_ptr<LevelManager>> loadGame();
    static bool saveExists();
    
private:
    static GameSaveData serializeGameState(const GameController& controller, const LevelManager& levelManager);
    static std::vector<SpellSaveData> serializeSpells(const Hand& hand);
    static void deserializeGameState(GameController& controller, LevelManager& levelManager, const GameSaveData& data, const std::vector<SpellSaveData>& spells);
    static void deserializeSpells(Hand& hand, const std::vector<SpellSaveData>& spells);
    static void validateSaveData(const GameSaveData& data);
};

#endif