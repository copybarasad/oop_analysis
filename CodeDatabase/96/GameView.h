#pragma once
#include <string>

template <typename Renderer>
class GameView {
private:
    Renderer renderer;

public:
    void update(const GameManager& gm, std::string state) {
        renderer.Render(gm, state);
    }

    void print(const std::string& msg) {
        renderer.Print(msg);
    }
};
