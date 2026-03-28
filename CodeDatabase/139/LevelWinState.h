#pragma once
#include "GameState.h"

class Game;

class LevelWinState : public GameState {
    Game* game;
    IConverter* converter;
public:
    LevelWinState(Game* game);
    void update() override;
    void render(IRenderer& renderer) override;
    IConverter* getConverter() override;
    ~LevelWinState();
};