#pragma once
#include "GameState.h"

class Game;

class PauseState : public GameState {
    Game* game;
    IConverter* converter;
public:
    PauseState(Game* game);
    void update() override;
    void render(IRenderer& renderer) override;
    IConverter* getConverter() override;
    ~PauseState();
};
