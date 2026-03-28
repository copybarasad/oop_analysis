#ifndef GAME_IRENDERER_H
#define GAME_IRENDERER_H

namespace Game {

class Game; 

// Интерфейс для отрисовщиков
class IRenderer {
public:
    virtual ~IRenderer() = default;
    virtual void render(const Game& game) = 0;
};

}

#endif
