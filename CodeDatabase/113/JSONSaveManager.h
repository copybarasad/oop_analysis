#ifndef JSONSAVEMANAGER_H
#define JSONSAVEMANAGER_H

#include "GameState.h"
#include "SaveDataValidator.h"
#include <string>
#include <nlohmann/json.hpp>

class JSONSaveManager {
public:
    static bool saveGameState(const GameState& gameState, const std::string& filename);
    static bool loadGameState(GameState& gameState, const std::string& filename);
    static bool saveExists(const std::string& filename);

    static std::string getLastError() { return m_lastError; }
    static void clearLastError() { m_lastError.clear(); }

private:
    static nlohmann::json gameStateToJSON(const GameState& gameState);
    static bool JSONToGameState(const nlohmann::json& j, GameState& gameState);

    static bool handleSaveError(const std::string& error);
    static bool handleLoadError(const std::string& error);
    static bool validateJSONStructure(const nlohmann::json& j);

    static std::string calculateChecksum(const std::string& data);
    static const std::string SECRET_KEY;

    static std::string m_lastError;
};

#endif // JSONSAVEMANAGER_H