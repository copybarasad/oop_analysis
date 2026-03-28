#ifndef GAME_RENDERER_INTERFACE_H
#define GAME_RENDERER_INTERFACE_H

#include "game_controller.h"
#include "game_enums.h"
#include <vector>
#include <string>

//Интерфейс для классов отрисовки игры. Параметр шаблона для GameVisualizationManager.
class IGameRenderer {
public:
    virtual ~IGameRenderer() = default;
    
    // Отрисовка состояний
    virtual void renderGameState(const GameController& game) const = 0;
    virtual void renderMainMenu() const = 0;
    virtual void renderLoadMenu(const std::vector<std::string>& saveFiles) const = 0;
    virtual void renderInGameMenu() const = 0;
    virtual void renderGameOverMenu(const GameController& game) const = 0;
    
    // Отрисовка событий
    virtual void renderLevelStart(int level, const LevelConfig& config) const = 0;
    virtual void renderLevelComplete(int level, int score) const = 0;
    virtual void renderVictory(int finalScore) const = 0;
    
    // Отрисовка сообщений
    virtual void renderMessage(const std::string& message) const = 0;
    virtual void renderError(const std::string& error) const = 0;
    
    // Отрисовка компонентов
    virtual void renderPlayerStats(const Player& player) const = 0;
    virtual void renderFieldGrid(const GameController& game) const = 0;
};

#endif