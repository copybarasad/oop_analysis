#ifndef GAME_CONTROLLER_HPP
#define GAME_CONTROLLER_HPP

#include "game_state.hpp"
#include "renderer.hpp"
#include "save_game.hpp"
#include <memory>
#include <string>

class GameController {
private:
    std::unique_ptr<GameState> gameState;
    Renderer renderer;
    SaveGame saveSystem;
    
    int selectedSpellIndex;
    bool isCastingSpell;
    bool playerTurnCompleted;
    
    char getCharacter() const;
    
public:
    GameController();
    
    void initializeNewGame();
    void processPlayerTurn();
    void processEnemyTurn();
    void nextTurn();
    
    void saveGame(const std::string& filename = "savegame.dat") const;
    bool loadGame(const std::string& filename = "savegame.dat");
    
    bool isGameActive() const;
    bool isGameOver() const;
    bool isLevelComplete() const;
    bool isPlayerTurnCompleted() const;
    
    int getCurrentLevel() const;
    int getCurrentTurn() const;
    int getScore() const;
    
private:
    void handleMove(char direction);
    void handleAttack(char direction);
    void handleSpellSelection(int spellNum);
    void handleSpellDirection(char direction);
    void handleWeaponSwitch();
    void handleSpellPurchase();
    void handleSkipTurn();
};

#endif