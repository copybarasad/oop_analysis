#ifndef IRENDER_STRATEGY_H
#define IRENDER_STRATEGY_H

#include <string>

class GameController;

class IRenderStrategy {
public:
    virtual ~IRenderStrategy() = default;
    virtual void renderGame(const GameController& controller) = 0;
    virtual void renderMenu(const std::string& menuText) = 0;
    virtual void renderMessage(const std::string& message) = 0;
    virtual void clearScreen() = 0;
};

#endif