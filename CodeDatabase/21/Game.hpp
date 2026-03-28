#pragma once
#include <unistd.h>
#include <ctime>
#include "GameField.hpp"
#include "entities/Player.hpp"
#include "IGameController.hpp"
#include "Config.hpp"
#include "FileHandler.hpp"
#include "SavesManager.hpp"
#include "SaveData.hpp"

#define GAME_SAVES_DIR "/Game/"

class Game : public IGameController{
private:
    SavesManager savesManager;
    GameField* field = nullptr;
    int gameID = 0;
public:
    Game() {
        startNewGame();
    }
    ~Game();
    void startNewGame() override;
    void stopGame() override;
    void saveGame(std::string saveName = "save") override;
    bool loadGame(std::string saveName) override;
    void nextLevel() override;
    void playerLevelUp(char attribute) override;
    bool performAnAction(char playerAction) override;
    std::vector<std::string> getSavesList(int start, int end) override;
    std::shared_ptr<PlayerData> getPlayerData() override;
    std::vector<EnemyData> getEnemyData() override;
    std::vector<wchar_t> getFieldData() override;
private:
    SaveData collectGameData();
    void setGameData(SaveData& data);
    int getGameID();
    void setGameID(int newGameID);
    void deleteField();
};
