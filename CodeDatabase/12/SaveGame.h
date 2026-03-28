#ifndef SAVEGAME_H
#define SAVEGAME_H

#include <vector>
#include <string>
#include <exception>

struct SaveGame {
    int playerX, playerY;
    int playerHealth;
    bool playerAlive;
    int playerMeleeDamage;
    int playerRangedDamage;
    bool playerIsMelee;
    int fieldWidth, fieldHeight;
    bool hasActiveTrap;
    int trapX, trapY, trapDamage;
    
    int currentLevel;
    
    struct EnemyData {
        int x, y;
        int health, damage;
        bool alive;
    };
    
    struct TowerData {
        int x, y;
        int range, health;
        int cooldown, currentCooldown;
        bool alive;
    };
    
    struct SpellData {
        std::string type;
        int param1, param2;
    };
    
    struct WallData {
        int x, y;
    };
    
    std::vector<EnemyData> enemies;
    std::vector<TowerData> towers;
    std::vector<SpellData> spells;
    std::vector<WallData> walls;
    
    int turnCount;
};

class SaveLoadException : public std::exception {
private:
    std::string message;
public:
    explicit SaveLoadException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};

class SaveSystem {
private:
    static const std::string SAVE_FILENAME;
    static const std::string SAVE_FILE_VERSION;
    
    static void validateSaveData(const SaveGame& data);
    static void validateFileHeader(std::ifstream& file);

public:
    static bool saveGame(const SaveGame& data);
    static bool loadGame(SaveGame& data);
    static bool saveExists();
    static void showSaveMenu(SaveGame& data);
    static void deleteSave();
};

#endif