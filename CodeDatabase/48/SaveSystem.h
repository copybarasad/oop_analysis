#ifndef SAVESYSTEM_H
#define SAVESYSTEM_H

#include <string>
#include <vector>

struct GameSaveData {
    // Player data
    std::string playerName;
    int playerHealth;
    int playerMaxHealth;
    int playerDamage;
    int playerScore;
    int playerLevel;
    int playerX;
    int playerY;
    int playerMana;
    int playerMaxMana;
    
    // Field data
    int fieldWidth;
    int fieldHeight;
    std::vector<std::vector<bool>> fieldPassable;
    std::vector<std::vector<bool>> fieldHasEnemy;
    std::vector<std::vector<bool>> fieldHasTower;
    
    // Enemies data
    struct EnemyData {
        int health;
        int maxHealth;
        int damage;
        int x;
        int y;
        bool alive;
    };
    std::vector<EnemyData> enemies;
    
    // Towers data
    struct TowerData {
        int health;
        int maxHealth;
        int x;
        int y;
        int attackRange;
        int attackDamage;
        bool alive;
    };
    std::vector<TowerData> towers;
    
    // Game state
    int currentTurn;
    int currentLevel;
    bool gameRunning;
};

class SaveSystem {
private:
    static const std::string SAVE_FILE_PATH;
    
public:
    static bool saveGame(const GameSaveData& data, const std::string& slotName = "default");
    static bool loadGame(GameSaveData& data, const std::string& slotName = "default");
    static std::vector<std::string> getAvailableSaves();
    static bool deleteSave(const std::string& slotName);
    static bool saveExists(const std::string& slotName);
};

#endif