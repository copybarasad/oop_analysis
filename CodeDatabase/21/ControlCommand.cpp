#include "ControlCommand.hpp"


void ControlCommand::mainMenuExecute(
    IGameController& gameController,
    GameState& currentState,
    GameState& prevState
) {
    if (this->controlCommand == StandardKeyboardLayout::KEYENTER) {
        if (GlobalMenuSelector::mainMenuSelected == 0) {
            gameController.startNewGame();
            currentState = GameState::InGame;
        } else if (GlobalMenuSelector::mainMenuSelected == 1) {
            gameController.startNewGame();
            currentState = GameState::LoadMenu;
            prevState = GameState::MainMenu;
        } else if (GlobalMenuSelector::mainMenuSelected == 2) {
            currentState = GameState::AutorsMenu;
        } else if (GlobalMenuSelector::mainMenuSelected == 3) {
            currentState = GameState::Exit;
        }
    }
    if (this->controlCommand == StandardKeyboardLayout::KEYESC) {
        currentState = GameState::Exit;
    }
}


void ControlCommand::loadMenuExecute(
    IGameController& gameController,
    GameState& currentState,
    GameState& prevState
) {
    if (this->controlCommand == StandardKeyboardLayout::KEYENTER) {
        std::vector<std::string> loadMenuOptions = gameController.getSavesList(GlobalMenuSelector::startListLoadMenu, GlobalMenuSelector::endListLoadMenu);
        std::string selectedSave = loadMenuOptions[GlobalMenuSelector::loadMenuSelected];
        try {
            if (gameController.loadGame(selectedSave)) {
                GlobalMenuSelector::startListLoadMenu = 0;
                GlobalMenuSelector::endListLoadMenu = 10;
                GlobalMenuSelector::loadMenuSelected = 0;
                GlobalMenuSelector::firstCallLoadMenu = true;
                GlobalMenuSelector::errMessage = "";
                currentState = GameState::InGame;
            }
            else {
                GlobalMenuSelector::errMessage = "failed to load save";
            }
        }
        catch(const std::exception& e) {
            GlobalMenuSelector::errMessage = e.what();
        }

    }
    if (this->controlCommand == StandardKeyboardLayout::KEYESC) {
        GlobalMenuSelector::startListLoadMenu = 0;
        GlobalMenuSelector::endListLoadMenu = 10;
        GlobalMenuSelector::loadMenuSelected = 0;
        GlobalMenuSelector::firstCallLoadMenu = true;
        currentState = prevState;
    }
}


void ControlCommand::inGameExecute(
    GameState& currentState
) {
    if (this->controlCommand == StandardKeyboardLayout::KEYESC) {
        currentState = GameState::PauseMenu;
    }
}


void ControlCommand::levelUpMenuExecute(
    IGameController& gameController,
    GameState& currentState
) {
    if (this->controlCommand == StandardKeyboardLayout::KEYENTER) {
        char choice = '1' + GlobalMenuSelector::levelUpSelected;
        gameController.playerLevelUp(choice);
        currentState = GameState::InGame;
        GlobalMenuSelector::levelUpSelected = 0;
    }
}


void ControlCommand::gameOverExecute(
    IGameController& gameController,
    GameState& currentState,
    GameState& prevState
) {
    if (this->controlCommand == StandardKeyboardLayout::KEYENTER) {
        if (GlobalMenuSelector::gameOverSelected == 0) {
            gameController.startNewGame();
            currentState = GameState::InGame;
        }
        else if (GlobalMenuSelector::gameOverSelected == 1) {
            currentState = GameState::LoadMenu;
            prevState = GameState::GameOver;
        }
        else if (GlobalMenuSelector::gameOverSelected == 2) {
            currentState = GameState::MainMenu;
        }
    }
}


void ControlCommand::pauseMenuExecute(
    IGameController& gameController,
    GameState& currentState,
    GameState& prevState
) {
    if (this->controlCommand == StandardKeyboardLayout::KEYENTER) {
        if (GlobalMenuSelector::pauseMenuSelected == 0) {
            currentState = GameState::InGame;
        }
        else if (GlobalMenuSelector::pauseMenuSelected == 1) {
            currentState = GameState::InGame;
            gameController.saveGame();
        }
        else if (GlobalMenuSelector::pauseMenuSelected == 2) {
            currentState = GameState::LoadMenu;
            prevState = GameState::PauseMenu;
        }
        else if (GlobalMenuSelector::pauseMenuSelected == 3) {
            currentState = GameState::MainMenu;
        }
        GlobalMenuSelector::pauseMenuSelected = 0;
    }
    if (this->controlCommand == StandardKeyboardLayout::KEYESC) {
        currentState = GameState::InGame;
        GlobalMenuSelector::pauseMenuSelected = 0;
    }
}


void ControlCommand::autorsMenuExecute(
    GameState& currentState
) {
    currentState = GameState::MainMenu;
}


void ControlCommand::execute(
    IGameController& gameController,
    GameState& currentState,
    GameState& prevState
) {
    switch (currentState) {
    case GameState::MainMenu:
        mainMenuExecute(
            gameController,
            currentState,
            prevState
        );
        break;
    case GameState::LoadMenu:
        loadMenuExecute(
            gameController,
            currentState,
            prevState
        );
        break;
    case GameState::InGame:
        inGameExecute(currentState);
        break;
    case GameState::LevelUpMenu:
        levelUpMenuExecute(
            gameController,
            currentState
        );
        break;
    case GameState::GameOver:
        gameOverExecute(
            gameController,
            currentState,
            prevState
        );
        break;
    case GameState::PauseMenu:
        pauseMenuExecute(
            gameController,
            currentState,
            prevState
        );
        break;
    case GameState::AutorsMenu:
        autorsMenuExecute(currentState);
        break;
    default:
        break;
    }
}