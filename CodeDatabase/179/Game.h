#ifndef GAME_H
#define GAME_H

#include "GameField.h"
#include "SpellHand.h"
#include "GameCommand.h"
#include "GameException.h"
#include "Player.h"
#include <memory>
#include <string>

class Game {
private:
    std::unique_ptr<GameField> currentField;
    std::shared_ptr<Player> player;
    SpellHand spellHand;
    bool gameRunning;
    int currentLevel;
    
public:
    Game();
    ~Game() = default;
    
    void startNewGame();
    bool isRunning() const;
    
    void processCommand(const GameCommand& command);
    void gameLoop();
    
    void saveGame(const std::string& filename);
    void loadGame(const std::string& filename);
    
    // Методы доступа для GameController и GameView
    GameField* getCurrentField() const;
    int getCurrentLevel() const;
    std::shared_ptr<Player> getPlayer() const;
    
private:
    void initializeLevel(int level);
    void playerTurn();
    void alliesTurn();
    void enemiesTurn();
    void buildingsTurn();
    void towersTurn();
    void checkGameState();
    void displayGameState();
    Position getSpellTarget();
    void levelUp();
};

#endif
