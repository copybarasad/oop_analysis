#ifndef GAMEINTERFACE_H
#define GAMEINTERFACE_H

#include "GameState.hpp"
#include "UpgradeSystem.hpp"
#include "SpellHand.hpp"
#include <string>
#include <memory>

class GameManager;

class GameInterface {
private:
    std::unique_ptr<GameManager> gameManager;
    
    void displayMainMenu();
    void displayGameState(const GameState& state);
    void displayField(const Field& field);
    void displayFieldWithTarget(const Field& field, int targetX, int targetY);
    void displayGameOver(const GameState& state);
    void displayVictory(const GameState& state);
    void displayLevelTransition(const GameState& state);
    void displayUpgradePhase(const GameState& state, UpgradeSystem& upgradeSystem);
    void displaySpellTargetSelection(const GameState& state, ISpell* spell, int targetX, int targetY);
    void displayRangedAttackTarget(const GameState& state, int targetX, int targetY);
    void displayMessage(const std::string& message);
    void clearScreen();
    
public:
    GameInterface();
    ~GameInterface();
    void run();
    
    void showMainMenu();
    void showGameState();
    void showGameOver();
    void showVictory();
    void showLevelTransition();
    void showUpgradePhase();
    void showSpellTargetSelection(ISpell* spell, int targetX, int targetY);
    void showRangedAttackTarget(int targetX, int targetY);
    void showMessage(const std::string& message);
};

#endif