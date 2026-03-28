#ifndef GAMEMANAGER_HPP
#define GAMEMANAGER_HPP

#include "../map/gamemap.hpp"
#include "movement.hpp"
#include "combat.hpp"
#include "enemymanager.hpp"
#include "../entities/player.hpp"
#include "magic.hpp"
#include "../entities/magetower.hpp"
#include "../entities/ally.hpp"
#include "../auxil/gameexc.hpp"
#include "../auxil/loadexc.hpp"
#include "../auxil/formexc.hpp"
#include "../auxil/file.hpp"
#include <vector>
#include <memory>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "../include/json.hpp"

using json = nlohmann::json;

class Game {
private:
    GameMap gameMap;
    std::unique_ptr<Player> player;
    std::vector<std::unique_ptr<Character>> allCharacters;
    int level = 1;

    Movement movement;
    Combat combat;
    EnemyManager enemyManager;
    MagicHand magichand;
public:
    Game(std::string name);
    Game(int width, int height);
    void mainCycle();
    void loadGame(json& j);
    void advance();
    
    void processPlayerTurn(const std::string& command);
    void processEnemyTurns();
    void updateMageTowers();
    void updateGameState();
    void saveGameState();

    void displayGame() const;
    bool isGameOver() const;
    bool playerWon() const;
    Player* getPlayer() const;
    int getLevel() const { return level; }
    bool advanceToNextLevel(); 

private:
    void initializeGame();
    void handlePlayerMovement(const std::string& direction);
    void handlePlayerAttack(const std::string& direction);
    void updateBurning();
};

#endif
