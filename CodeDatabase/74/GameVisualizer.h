#pragma once

template <typename TRenderer>
class GameVisualizer {
private:
    TRenderer renderer;
    Game& game;

public:
    GameVisualizer(Game& g) : game(g) {}

    void update() {
        renderer.render(game);
    }

    void setMessage(const std::string& msg) { game.setCurrentMessage(msg); }
    void setAim(bool active, const Coords& pos, bool splash = false, int r = 0, bool trap = false) {
        game.setAim(active, pos, splash, r, trap);
    }
    void setShowHand(bool show) { game.setShowHand(show); }
};