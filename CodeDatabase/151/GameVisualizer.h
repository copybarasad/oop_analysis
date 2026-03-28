#ifndef GAME_VISUALIZER_H
#define GAME_VISUALIZER_H

template<typename Renderer>
class GameVisualizer {
    Renderer renderer;

public:
    void update(const GameField& field, const Player& player) const {
        renderer.render(field, player);
    }
};

#endif