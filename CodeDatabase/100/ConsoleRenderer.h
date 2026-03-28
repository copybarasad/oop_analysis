#pragma once
#include "Renderer.h"  
#include "../game_objects/GameField.h"
#include "../game_objects/Player.h"
#include "../game_objects/Enemy.h"
#include "../game_objects/EnemyBuilding.h"
#include "../game_objects/Ally.h"
#include "../game_objects/EnemyTower.h"
#include <vector>
#include <iostream>
#include "../game_objects/PlayerHand.h"

namespace Colors {
    const std::string RESET = "\033[0m";
    const std::string PLAYER = "\033[1;32m";
    const std::string ENEMY = "\033[1;31m";
    const std::string WALL = "\033[1;37m"; 
    const std::string EMPTY = "\033[1;34m";
    const std::string SLOW = "\033[1;35m";
    const std::string CYAN = "\033[36m";
    const std::string ORANGE = "\033[38;5;208m";
    const std::string RED = "\033[31m";  
    const std::string GREEN = "\033[32m";  
    const std::string BRIGHT_GREEN = "\033[38;5;10m";
    const std::string YELLOW = "\033[33m";
    const std::string DARK_RED = "\033[38;5;88m";
    const std::string GRAY = "\033[37m";
    const std::string PINK = "\033[95m";
    const std::string BROWN = "\033[38;5;130m";
};

class ConsoleRenderer : public Renderer {
public:
    void drawField(const GameField& field, 
                   const std::vector<Character*>& characters,
                   const std::vector<EnemyBuilding*>& buildings,
                   const std::vector<EnemyTower*>& towers,
                   const std::vector<Ally*>& allies) override;
    
    void drawPlayerInfo(const Player& player, const PlayerHand& hand) override;
    
    void drawMessages(const std::vector<std::string>& messages) override;

    void clearScreen() override;
};