#ifndef GAMESAVE_H
#define GAMESAVE_H

#include <string>
#include <vector>
#include <tuple>
#include <fstream>

class Game;

class GameSave {
public:
    class SaveException : public std::exception {
    private:
        std::string message;
    public:
        SaveException(const std::string& msg) : message(msg) {}
        const char* what() const noexcept override { return message.c_str(); }
    };
    
    class LoadException : public std::exception {
    private:
        std::string message;
    public:
        LoadException(const std::string& msg) : message(msg) {}
        const char* what() const noexcept override { return message.c_str(); }
    };

private:
    struct SaveData {
        int level;
        
        // Player data
        int playerHealth;
        int playerMaxHealth;
        int playerMeleeDamage;
        int playerRangedDamage;
        int playerMana;
        int playerMaxMana;
        int playerScore;
        int playerPosX;
        int playerPosY;
        std::vector<std::string> spellTypes;
        
        // Map data
        int mapWidth;
        int mapHeight;
        std::vector<std::vector<char>> mapGrid;
        
        // Game objects
        std::vector<std::tuple<int, int, int, int>> enemies; // x, y, health, damage
        std::vector<std::tuple<int, int, int>> towers;       // x, y, health
        std::vector<std::tuple<int, int>> traps;             // x, y
        std::vector<std::tuple<int, int, int>> allies;       // x, y, health
    };

    std::string filename;

    SaveData serializeGame(const Game& game);
    void deserializeGame(Game& game, const SaveData& data);
    void writeToFile(const SaveData& data);
    SaveData readFromFile();

public:
    GameSave(const std::string& file = "game_save.dat");
    void saveGame(const Game& game);
    void loadGame(Game& game);
    
    static std::vector<std::string> getSaveFiles();
    static bool deleteSave(const std::string& filename);
    bool saveExists() const;
};

#endif