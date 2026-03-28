#pragma once
#include "../Config.hpp"
#include "../GameState.hpp"
#include "../IGameController.hpp"


template <class Render>
class Visualizer {
private:
    Render& renderer;
    IGameController* gameController = nullptr;
public:
    Visualizer(Render& renderer, IGameController* gc) : renderer(renderer), gameController(gc) {};
    ~Visualizer() = default;
    void display(GameState& currentState) {
        renderer.updateTermSize();
        switch (currentState) {
            case GameState::MainMenu:
                renderer.drawMainMenu();
                break;
            case GameState::InGame: {
                std::vector<wchar_t> fieldChars = gameController->getFieldData();
                std::shared_ptr<PlayerData> playerData = gameController->getPlayerData();
                std::vector<EnemyData> enemyData = gameController->getEnemyData();
                renderer.drawInGame(fieldChars, playerData, enemyData);
                break;
            }
            case GameState::LevelUpMenu: {
                std::shared_ptr<PlayerData> playerData = gameController->getPlayerData();
                std::vector<EnemyData> enemyData = gameController->getEnemyData();
                renderer.drawLevelUpMenu(playerData, enemyData);
                break;
            }
            case GameState::GameOver:
                renderer.drawGameOverMenu();
                break;
            case GameState::PauseMenu:
                renderer.drawPauseMenu();
                break;
            case GameState::AutorsMenu:
                renderer.drawAutorsMenu();
                break;
            case GameState::LoadMenu: {
                std::vector<std::string> loadMenuOptions = gameController->getSavesList(GlobalMenuSelector::startListLoadMenu, GlobalMenuSelector::endListLoadMenu);
                renderer.drawLoadMenu(loadMenuOptions);
                break;
            }
            default:
                break;
        }
    }
};