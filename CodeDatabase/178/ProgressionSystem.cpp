#include "ProgressionSystem.h"
#include "Game.h"
#include <iostream>
#include <limits>

void ProgressionSystem::handleLevelTransition(Game& game) {
    auto& session = game.getGameSession();
    if (!session.getController() || !session.getLevelManager()) {
        game.setState(GameState::MAIN_MENU);
        return;
    }
    
    GameController& controller = *session.getController();
    LevelManager& levelManager = *session.getLevelManager();
    
    std::cout << "\n=== LEVEL TRANSITION ===" << std::endl;
    
    applyLevelUp(controller);
    
    showUpgradeMenu(controller, levelManager);
    
    levelManager.proceedToNextLevel(controller);
    if (levelManager.isGameCompleted()) {
        game.setState(GameState::VICTORY);
    } else {
        levelManager.initializeLevel(controller);
        game.setState(GameState::PLAYING);
    }
}

void ProgressionSystem::applyLevelUp(GameController& controller) {
    PlayerProgression::applyLevelUp(controller.getPlayerMutable(), controller.getHand());
}

void ProgressionSystem::showUpgradeMenu(GameController& controller, LevelManager& levelManager) {
    bool upgradeSelected = false;
    
    while (!upgradeSelected) {
        PlayerProgression::showUpgradeMenu(controller.getPlayerMutable());
        
        char choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        upgradeSelected = processUpgradeChoice(controller, levelManager, choice);
        
        if (!upgradeSelected) {
            std::cout << "Invalid choice or not enough coins! Try again." << std::endl;
        }
    }
}

bool ProgressionSystem::processUpgradeChoice(GameController& controller, LevelManager& levelManager, char choice) {
    switch (choice) {
        case '1':
            if (PlayerProgression::upgradePlayerStat(controller.getPlayerMutable(), "health", GameConfig::getUpgradeCost())) {
                levelManager.setUpgradeStates(true, false, false);
                return true;
            }
            break;
            
        case '2':
            if (PlayerProgression::upgradePlayerStat(controller.getPlayerMutable(), "melee", GameConfig::getUpgradeCost())) {
                levelManager.setUpgradeStates(false, true, false);
                return true;
            }
            break;
            
        case '3':
            if (PlayerProgression::upgradePlayerStat(controller.getPlayerMutable(), "ranged", GameConfig::getUpgradeCost())) {
                levelManager.setUpgradeStates(false, false, true);
                return true;
            }
            break;
            
        case '4':
            std::cout << "Skipping upgrade." << std::endl;
            return true;
            
        default:
            break;
    }
    
    return false;
}