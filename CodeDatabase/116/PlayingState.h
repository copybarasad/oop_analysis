#pragma once
#include "GameState.h"
#include <iostream>

class PlayingState : public GameState{
public:
    PlayingState() = default;
    ~PlayingState() = default;
    void handleInput(GameContext& context) override;
    void render(GameContext& context) override;
    std::string getName() const override { return "Playing"; }
    void update(GameContext& context);
};