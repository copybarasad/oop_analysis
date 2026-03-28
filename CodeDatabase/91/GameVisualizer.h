#ifndef VISUALIZERS_GAMEVISUALIZER_H
#define VISUALIZERS_GAMEVISUALIZER_H

#include "../Renderers/IGameRenderer.h"

template<typename T> class GameVisualizer {
    private:
        T renderer_;

    public:
        template<typename... Args>
        explicit GameVisualizer(Args&&... args);

        void visualize(const Game& game);

        IGameRenderer* getRenderer();
};

template<typename T>
template<typename... Args>
GameVisualizer<T>::GameVisualizer(Args&&... args) : renderer_(std::forward<Args>(args)...) {
}

template<typename T>
void GameVisualizer<T>::visualize(const Game& game) {
    renderer_.render(game);
}

template<typename T>
IGameRenderer* GameVisualizer<T>::getRenderer() {
    return &renderer_;
}

#endif