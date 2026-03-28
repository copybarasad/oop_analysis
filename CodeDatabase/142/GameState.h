#pragma once
#include <string>
#include <vector>
#include <utility>
#include <optional>

class GameState {
public:
    struct SpellData {
        int type{0}; // 0 = DirectDamage, 1 = AreaDamage
        int radius{0};
        int damage{0};
    };

    GameState();

    // Builder-style setters
    void setPlayer(int x, int y, int hp, int maxHp, int score, int meleeDmg, int rangedDmg, int combatMode);
    void setHandMaxSize(int size);
    void addSpell(const SpellData& spell);
    void setFieldSize(int width, int height);
    void addWall(int x, int y);
    void addEnemy(int x, int y, int hp, int damage);
    void setProgress(int level, int initialEnemies, int killed);

    // Getters
    int getPlayerX() const;
    int getPlayerY() const;
    int getPlayerHP() const;
    int getPlayerMaxHP() const;
    int getPlayerScore() const;
    int getMeleeDamage() const;
    int getRangedDamage() const;
    int getCombatMode() const;
    int getHandMaxSize() const;
    const std::vector<SpellData>& getSpells() const;
    int getFieldWidth() const;
    int getFieldHeight() const;
    const std::vector<std::pair<int, int>>& getWalls() const;
    const std::vector<std::pair<int, int>>& getEnemies() const;
    const std::vector<int>& getEnemyHP() const;
    const std::vector<int>& getEnemyDamage() const;
    int getCurrentLevel() const;
    int getInitialEnemyCount() const;
    int getEnemiesKilled() const;

    // Serialization
    bool saveToFile(const std::string& filename, std::string& error) const;
    static std::optional<GameState> loadFromFile(const std::string& filename, std::string& error);

    void clear();

private:
    // Player state
    int playerX;
    int playerY;
    int playerHP;
    int playerMaxHP;
    int playerScore;
    int meleeDamage;
    int rangedDamage;
    int combatMode; // 0 = Melee, 1 = Ranged
    int handMaxSize;

    // Hand spells (type, radius, damage)
    std::vector<SpellData> spells;

    // Field state
    int fieldWidth;
    int fieldHeight;
    std::vector<std::pair<int, int>> walls; // (x, y) positions
    std::vector<std::pair<int, int>> enemies; // (x, y) positions
    std::vector<int> enemyHP; // HP for each enemy
    std::vector<int> enemyDamage; // Damage for each enemy

    // Game state
    int currentLevel;
    int initialEnemyCount;
    int enemiesKilled;

    bool validate(std::string& error) const;
};

