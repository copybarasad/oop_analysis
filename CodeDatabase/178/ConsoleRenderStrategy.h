#ifndef CONSOLE_RENDER_STRATEGY_H
#define CONSOLE_RENDER_STRATEGY_H

#include "IRenderStrategy.h"
#include <string>

class ConsoleRenderStrategy : public IRenderStrategy {
public:
    void renderGame(const GameController& controller) override;
    void renderMenu(const std::string& menuText) override;
    void renderMessage(const std::string& message) override;
    void clearScreen() override;
    
private:
    void printSeparator() const;
};
#endif