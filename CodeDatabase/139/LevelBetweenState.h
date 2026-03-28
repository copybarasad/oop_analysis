#pragma once
#include "GameState.h"

class Game;

class LevelBetweenState : public GameState {
    Game* game;
    IConverter* converter;
public:
    LevelBetweenState(Game* game);
    void update() override;
    void render(IRenderer& renderer) override;
    IConverter* getConverter() override;
    ~LevelBetweenState();
};