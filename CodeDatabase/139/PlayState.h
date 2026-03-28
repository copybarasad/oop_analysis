#pragma once
#include "GameState.h"


class Game;

class PlayState : public GameState {
    Game* game;
    IConverter* converter;
public:
    PlayState(Game* game);
    void update() override;
    void render(IRenderer& renderer) override;
    IConverter* getConverter() override;
    ~PlayState();
};