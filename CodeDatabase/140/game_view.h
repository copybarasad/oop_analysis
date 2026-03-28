#pragma once

class Game;

template <typename TRenderer>
class GameView {
public:
    explicit GameView(Game& game);

    TRenderer& get_renderer() { return renderer; }

    void on_game_started();
    void on_turn();
    void on_game_over();

private:
    Game&     game;
    TRenderer renderer;
};
