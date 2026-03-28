#ifndef GAME_CONTROL_H
#define GAME_CONTROL_H

#include "Game.h"
#include "Command.h"
#include "game_renderer.h"
#include <memory>

template<typename InputType, typename RendererType = ConsoleRenderer>
class GameControl {
private:
    std::unique_ptr<Game> game;
    InputType input;
    std::unique_ptr<RendererType> renderer;
    
public:
    GameControl(Game* G);
    
    nlohmann::json toJson() const;
    void fromJson(const nlohmann::json& j);
    void newField(bool flag);
    bool player_move(const std::string& move);
    void enemy_move();
    void building_move();
    void spell_tower_move();
    bool game_on();
    void spellUp();
    
    void setRenderer(std::unique_ptr<RendererType> newRenderer);
    RendererType* getRenderer() const;
    
    int getCurrentTurn() const { return game->getCurrentTurn(); }
    int getLevel() const { return game->getLevel(); }
    bool isGameOver() const { return game->isGameOver(); }
    
private:
    void renderFullGameState();
    void renderGameInfo();
    void renderField();
    void renderMessage(const std::string& message);
    void clearScreen();
};

#include "game_control.tpp"

#endif