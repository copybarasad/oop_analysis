#ifndef GAME_GAMEVISUALIZER_H
#define GAME_GAMEVISUALIZER_H

#include "IObserver.h"
#include <memory>

namespace Game {

class Game; 

// Шаблонный класс-визуализатор
template <typename Renderer>
class GameVisualizer : public IObserver {
public:
    GameVisualizer(const Game& game) : gameInstance(game) {}

    void onNotify(const Event& event) override {
        // Перерисовываем игру на любое событие
        renderer.render(gameInstance);
    }

private:
    const Game& gameInstance;
    Renderer renderer;
};

}

#endif
