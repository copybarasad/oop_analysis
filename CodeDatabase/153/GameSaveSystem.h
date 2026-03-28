#ifndef GAMESAVESYSTEM_H
#define GAMESAVESYSTEM_H

#include "GameState.h"
#include <string>
#include <memory>

class GameStateSerializer;

class GameSaveSystem {
private:
    std::string defaultSaveFile = "game_save.txt";
    std::unique_ptr<GameStateSerializer> serializer;

    int calculateChecksum(const std::string& data) const;
    bool validateChecksum(const std::string& data) const;

public:
    GameSaveSystem();
    explicit GameSaveSystem(const std::string& saveFileName);
    ~GameSaveSystem();

    void saveGame(const GameState& gameState) const;
    void saveGame(const GameState& gameState, const std::string& filename) const;
    bool loadGame(GameState& gameState) const;
    bool loadGame(GameState& gameState, const std::string& filename) const;

    bool saveExists() const;
    bool saveExists(const std::string& filename) const;

    void setDefaultSaveFile(const std::string& filename);
    std::string getDefaultSaveFile() const;
    bool deleteSaveFile() const;
    bool deleteSaveFile(const std::string& filename) const;
};

#endif 