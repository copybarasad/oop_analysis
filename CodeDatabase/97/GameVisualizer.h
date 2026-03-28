#ifndef UNTITLED_GAMEVISUALIZER_H
#define UNTITLED_GAMEVISUALIZER_H

#include "GameState.h"

template <class RendererT>
class GameVisualizer {
public:
    GameVisualizer() = default;

    void onGameUpdated(const GameState& state) {
        renderer_.draw(state);
    }

private:
    RendererT renderer_;
};


#endif // UNTITLED_GAMEVISUALIZER_H
