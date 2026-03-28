#include "PlayerActionCommand.hpp"


void PlayerActionCommand::execute(
    IGameController& gameController,
    GameState& currentState,
    GameState& prevState
) {
    (void)prevState;
    bool consumed = false;

    switch (currentState) {
    case GameState::InGame:
        if (this->actionSymbol >= StandardKeyboardLayout::KEY1 && this->actionSymbol <= StandardKeyboardLayout::KEY4) {
            gameController.performAnAction((char)('0' + (this->actionSymbol - '1')));
            return;
        }
        consumed = !gameController.performAnAction(this->actionSymbol);
        if (consumed) {
            currentState = GameState::GameOver;
        }
        break;
    case GameState::LoadMenu:
        if (this->actionSymbol == StandardKeyboardLayout::KEYDOWN) {
            if (GlobalMenuSelector::loadMenuSelected == 0 && GlobalMenuSelector::startListLoadMenu > 0) {
                GlobalMenuSelector::startListLoadMenu = std::max(0, GlobalMenuSelector::startListLoadMenu - 10);
                GlobalMenuSelector::endListLoadMenu = GlobalMenuSelector::startListLoadMenu + 10;
                std::vector<std::string> loadMenuOptions = gameController.getSavesList(GlobalMenuSelector::startListLoadMenu, GlobalMenuSelector::endListLoadMenu);
                GlobalMenuSelector::loadMenuSelected = (int)loadMenuOptions.size() - 1;
            } else {
                GlobalMenuSelector::loadMenuSelected = std::max(0, GlobalMenuSelector::loadMenuSelected - 1);
            }
        }
        if (this->actionSymbol == StandardKeyboardLayout::KEYUP) {
            if (GlobalMenuSelector::loadMenuSelected == (int)gameController.getSavesList(GlobalMenuSelector::startListLoadMenu, GlobalMenuSelector::endListLoadMenu).size() - 1) {
                int totalSaves = (int)gameController.getSavesList(0,-1).size();
                if (GlobalMenuSelector::endListLoadMenu < totalSaves) {
                    GlobalMenuSelector::startListLoadMenu = GlobalMenuSelector::endListLoadMenu;
                    GlobalMenuSelector::endListLoadMenu = std::min(GlobalMenuSelector::startListLoadMenu + 10, totalSaves);
                    std::vector<std::string> loadMenuOptions = gameController.getSavesList(GlobalMenuSelector::startListLoadMenu, GlobalMenuSelector::endListLoadMenu);
                    GlobalMenuSelector::loadMenuSelected = 0;
                }
            } else {
                GlobalMenuSelector::loadMenuSelected = std::min((int)gameController.getSavesList(GlobalMenuSelector::startListLoadMenu, GlobalMenuSelector::endListLoadMenu).size() - 1, GlobalMenuSelector::loadMenuSelected + 1);
            }
        }
        break;
    case GameState::GameOver:
        if (this->actionSymbol == StandardKeyboardLayout::KEYDOWN) {
            GlobalMenuSelector::gameOverSelected = (GlobalMenuSelector::gameOverSelected + 1) % GlobalMenuSelector::gameOverMod;
        }
        if (this->actionSymbol == StandardKeyboardLayout::KEYUP) {
            GlobalMenuSelector::gameOverSelected = (GlobalMenuSelector::gameOverSelected - 1 + GlobalMenuSelector::gameOverMod) % GlobalMenuSelector::gameOverMod;
        }
        break;
    case GameState::LevelUpMenu:
        if (this->actionSymbol == StandardKeyboardLayout::KEYDOWN) {
            GlobalMenuSelector::levelUpSelected = (GlobalMenuSelector::levelUpSelected + 1) % GlobalMenuSelector::levelUpMenuMod;
        }
        if (this->actionSymbol == StandardKeyboardLayout::KEYUP) {
            GlobalMenuSelector::levelUpSelected = (GlobalMenuSelector::levelUpSelected - 1 + GlobalMenuSelector::levelUpMenuMod) % GlobalMenuSelector::levelUpMenuMod;
        }
        break;
    case GameState::MainMenu:
        if (this->actionSymbol == StandardKeyboardLayout::KEYDOWN) {
            GlobalMenuSelector::mainMenuSelected = (GlobalMenuSelector::mainMenuSelected + 1) % GlobalMenuSelector::mainMenuMod;
        }
        if (this->actionSymbol == StandardKeyboardLayout::KEYUP) {
            GlobalMenuSelector::mainMenuSelected = (GlobalMenuSelector::mainMenuSelected - 1 + GlobalMenuSelector::mainMenuMod) % GlobalMenuSelector::mainMenuMod;
        }
        break;
    case GameState::PauseMenu:
        if (this->actionSymbol == StandardKeyboardLayout::KEYDOWN) {
            GlobalMenuSelector::pauseMenuSelected = (GlobalMenuSelector::pauseMenuSelected + 1) % GlobalMenuSelector::pauseMenuMod;
        }
        if (this->actionSymbol == StandardKeyboardLayout::KEYUP) {
            GlobalMenuSelector::pauseMenuSelected = (GlobalMenuSelector::pauseMenuSelected - 1 + GlobalMenuSelector::pauseMenuMod) % GlobalMenuSelector::pauseMenuMod;
        }
        break;
    default:
        break;
    }
    auto pdata = gameController.getPlayerData();
    if (pdata->levelIncreased) {
        currentState = GameState::LevelUpMenu;
    }
}