#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <vector>
#include <tuple>
#include <string>
#include <utility>

struct GameState { 
    int level = 1;
    int playerHealth = 100;
    int playerScore = 0;
    int playerMana = 100;
    int enemiesDefeated = 0;
    int enemyHealth = 50;
    int enemyLevel = 1;
    
    int playerX = -1;
    int playerY = -1;
    int enemyX = -1;
    int enemyY = -1;
     
    int fieldWidth = 15;
    int fieldHeight = 15;
     
    std::vector<std::pair<int, int>> obstacles;
    std::vector<std::tuple<int, int, int>> towers;    
    std::vector<std::pair<std::string, int>> spells; 
    std::vector<std::tuple<int, int, int, bool>> traps;  
};

#endif