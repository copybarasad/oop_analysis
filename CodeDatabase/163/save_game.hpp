#ifndef SAVE_GAME_HPP
#define SAVE_GAME_HPP

#include <string>
#include <vector>
#include <memory>

class GameState;

class SaveGame {
private:
    struct SaveData {
        int currentTurn;
        int currentLevel;
        int score;
        int playerX, playerY;
        int playerHP, playerMaxHP;
        std::vector<int> enemyPositionsX;
        std::vector<int> enemyPositionsY;
        std::vector<int> enemyHP;
        std::vector<int> buildingPositionsX;
        std::vector<int> buildingPositionsY;
        std::vector<int> buildingHP;
    };
    
    std::string saveFileName;
    
public:
    SaveGame(const std::string& filename = "savegame.dat");
    
    bool save(const GameState& gameState);
    bool load(GameState& gameState);
    
    static bool saveExists(const std::string& filename = "savegame.dat");
    static std::vector<std::string> listSaves();
    
private:
    void serialize(const GameState& gameState, SaveData& data) const;
    void deserialize(GameState& gameState, const SaveData& data) const;
    bool writeToFile(const SaveData& data) const;
    bool readFromFile(SaveData& data) const;
};

#endif