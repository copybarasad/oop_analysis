#pragma once
#include "GameState.h"

class Game;

class MenuState : public GameState {
    Game* game;
    IConverter* converter;
public:
    MenuState(Game* game);
    void update() override;
    void render(IRenderer& renderer) override;
    IConverter* getConverter() override;
    ~MenuState();
};