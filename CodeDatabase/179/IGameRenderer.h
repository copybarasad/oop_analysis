#ifndef IGAMERENDERER_H
#define IGAMERENDERER_H

#include "GameField.h"
#include <string>

class IGameRenderer {
public:
    virtual ~IGameRenderer() = default;
    
    virtual void renderField(const GameField& field) = 0;
    virtual void renderGameState(const GameField& field, int level, int score) = 0;
    virtual void renderHelp() = 0;
    virtual void renderMessage(const std::string& message) = 0;
    virtual void clearScreen() = 0;
};

#endif
