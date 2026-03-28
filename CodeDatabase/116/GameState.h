#pragma once
#include <string>

class GameContext;

class GameState {
public:
    virtual ~GameState() = default;

    virtual void handleInput(GameContext& context) = 0;
    virtual void render(GameContext& context) = 0;
    
    virtual std::string getName() const = 0;
};