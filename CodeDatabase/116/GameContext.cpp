#include "GameContext.h"
#include "MainMenuState.h"
#include "PlayingState.h"
#include "PauseState.h"
#include "GameOverState.h"
#include "LevelTransitionState.h"

GameContext::GameContext() : controller(nullptr), inputHandler(), statsRenderer(), fieldRenderer(), levelManager(), isRunning(true), gameRunning(true), currentLevel(1), saveLoadManager() {
    transitionTo("MainMenu");
}

void GameContext::transitionTo(const std::string& stateName){
    if (stateName == "MainMenu") {
        currentState = std::make_unique<MainMenuState>();
    } else if (stateName == "Playing") {
        currentState = std::make_unique<PlayingState>();
    } else if (stateName == "Pause") {
        currentState = std::make_unique<PauseState>();
    } else if (stateName == "GameOver") {
        currentState = std::make_unique<GameOverState>(false);
    } else if (stateName == "LevelTransition") {
        currentState = std::make_unique<LevelTransitionState>();
    }
}

void GameContext::initializeNewLevel(bool restartGame) {
    LevelConfig config = levelManager.getCurrentLevelConfig();

    std::unique_ptr<Player> savedPlayer = nullptr;
    if (controller) {
        savedPlayer = controller->getWorld().extractPlayer();
        
        if (savedPlayer && savedPlayer->getHealth() > 0 && !restartGame) {
            savedPlayer->setPosition(config.width/2, config.height/2);
        }
    }

    controller = std::make_unique<GameControl>(
        config.width, config.height,
        config.enemies, config.slowingCells,
        config.wallsCount, config.fortressesCount, config.lvl
    );

    if (savedPlayer && savedPlayer->getHealth() > 0  && !restartGame) {
        controller->getWorld().setPlayer(std::move(savedPlayer));
    }
    
    currentLevel = levelManager.getCurrentLevelNumber();
}


void GameContext::completeCurrentLevel() {
    if (levelManager.hasMoreLevels()) {
        levelManager.goToNextLevel();
        transitionTo("LevelTransition");
    } else {
        transitionTo("GameOver");
    }
}

void GameContext::handleInput() {
    if (currentState) {
        currentState->handleInput(*this);
    }
}

void GameContext::render() {
    if (currentState) {
        currentState->render(*this);
    }
}