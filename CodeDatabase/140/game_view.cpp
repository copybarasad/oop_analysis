#include "game_view.h"
#include "game.h"
#include "field.h"
#include "draw.h"

template class GameView<Renderer>;

template <typename TRenderer>
GameView<TRenderer>::GameView(Game& game)
    : game(game),
      renderer()
{}

template <typename TRenderer>
void GameView<TRenderer>::on_game_started() {
    renderer.render_help();
}

template <typename TRenderer>
void GameView<TRenderer>::on_turn() {
    if (game.get_field()) {
        renderer.render_field(*game.get_field());
    }
}

template <typename TRenderer>
void GameView<TRenderer>::on_game_over() {
    if (game.get_field()) {
        renderer.render_game_over(*game.get_field());
    }
}