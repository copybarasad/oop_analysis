#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>

class GameState {
public:
    int currentLevel;
    // Данные поля
    int width, height;
    std::vector<std::vector<int>> grid;  // 0 = Empty, 1 = Wall
    
    // Данные игрока
    int playerX, playerY;
    int playerHealth;
    int playerMana;
    int playerScore;
    bool playerCombatMode;
    
    // Данные врагов
    struct EnemyData {
        int x, y;
        int health;
        bool isAlive;
    };
    std::vector<EnemyData> enemies;
    
    // Сохранение
    void saveToFile(const std::string& filename);
    
    // Загрузка  
    void loadFromFile(const std::string& filename);
    
private:
    void validateState() const;
};

#endif