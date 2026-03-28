#ifndef CONSOLE_GAME_RENDERER_H
#define CONSOLE_GAME_RENDERER_H

#include "IGameRenderer.h"
#include <string>

class Game;

class ConsoleRenderer : public IGameRenderer {
public:
    void clear() override;

    void showGame(std::shared_ptr<Game>& game, const std::string& message) override;

    void showMessage(const std::string& message) override;
};

#endif
