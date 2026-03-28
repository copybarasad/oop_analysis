#ifndef GAME_RENDERER_H
#define GAME_RENDERER_H

#include <string>

class Game;

class GameRenderer {
public:
    virtual ~GameRenderer() = default;
    virtual void renderField(const Game& game) = 0;
    virtual void renderPlayerStatus(const Game& game) = 0;
    virtual void renderSpellList(const Game& game) = 0;
    virtual void renderMessage(const std::string& message) = 0;
    virtual void renderControls() = 0;
};

#endif