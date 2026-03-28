#pragma once
#include "GameState.h"

class Game;

class GameOverState : public GameState {
    Game* game;
    IConverter* converter;
public:
    GameOverState(Game* game);
    void update() override;
    void render(IRenderer& renderer) override;
    IConverter* getConverter() override;
    ~GameOverState();
};