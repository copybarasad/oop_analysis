#ifndef GAME_CONSOLERENDERER_H
#define GAME_CONSOLERENDERER_H

#include "IRenderer.h"

namespace Game {

class ConsoleRenderer : public IRenderer {
public:
    void render(const Game& game) override;

private:
    void displayGameStatus(const Game& game) const;
    void displayMap(const Game& game) const;
};

}

#endif
