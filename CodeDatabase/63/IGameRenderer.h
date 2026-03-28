#ifndef IGAME_RENDERER_H
#define IGAME_RENDERER_H

#include <string>

class Game;

class IGameRenderer {
public:
    virtual ~IGameRenderer() = default;

    virtual void clear() = 0;

    virtual void showGame(std::shared_ptr<Game>& game, const std::string& message) = 0;

    virtual void showMessage(const std::string& text) = 0;
};

#endif
