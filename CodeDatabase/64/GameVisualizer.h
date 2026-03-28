#ifndef GAME_VISUALIZER_H
#define GAME_VISUALIZER_H

template <typename Renderer>
class GameVisualizer {
private:
    Renderer renderer;

public:
    void draw(const Game& game) {
        renderer.render(game);
    }

    void showMessage(const std::string& msg) {
        renderer.printMessage(msg);
    }
};

#endif