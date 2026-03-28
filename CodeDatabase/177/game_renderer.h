#ifndef GAME_RENDERER_H
#define GAME_RENDERER_H

#include "game_controller.h"
#include "game_renderer_interface.h"
#include <iostream>
#include <vector>
#include <string>

//Реализация IGameRenderer для консольного вывода.
class GameRenderer : public IGameRenderer {
private:
    std::ostream& outputStream;
    
    // Вспомогательные методы
    void renderField(const GameController& game) const;
    void renderPlayerInfo(const Player& player) const;
    void renderSpells(const Player& player) const;
    void renderEnemiesInfo(const GameController& game) const;
    void renderCommands() const;
    void renderSeparator(int width = 50) const;
    
    // Отрисовка игрового поля
    char getCellSymbol(const GameController& game, int x, int y) const;
    void renderFieldHeader(int width) const;
    void renderFieldFooter(int width) const;
    
public:
    explicit GameRenderer(std::ostream& stream = std::cout);
    
    // Реализация интерфейса IGameRenderer
    void renderGameState(const GameController& game) const override;
    void renderMainMenu() const override;
    void renderLoadMenu(const std::vector<std::string>& saveFiles) const override;
    void renderInGameMenu() const override;
    void renderGameOverMenu(const GameController& game) const override;
    void renderLevelStart(int level, const LevelConfig& config) const override;
    void renderLevelComplete(int level, int score) const override;
    void renderVictory(int finalScore) const override;
    void renderMessage(const std::string& message) const override;
    void renderError(const std::string& error) const override;
    void renderPlayerStats(const Player& player) const override;
    void renderFieldGrid(const GameController& game) const override;
};

#endif