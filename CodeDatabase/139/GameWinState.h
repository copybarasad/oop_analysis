#pragma once
#include "GameState.h"

class Game;

class GameWinState : public GameState {
    Game* game;
    IConverter* converter;
public:
    GameWinState(Game* game);
    void update() override;
    void render(IRenderer& renderer) override;
    IConverter* getConverter() override;
    ~GameWinState();
};