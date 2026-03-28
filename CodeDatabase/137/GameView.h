#ifndef GAME_VIEW_H
#define GAME_VIEW_H

#include "Game.h"
#include <memory>


template<typename RendererType>
class GameView {
public:
    GameView(RendererType& renderer, const Game& game)
        : renderer_(renderer),
          game_(game),
          last_state_(game.GetState()) {}

    void Update() {
        GameState current_state = game_.GetState();
        if (current_state != last_state_) {
            OnStateChanged(current_state);
            last_state_ = current_state;
        }

        renderer_.Render(game_);
    }

    void HandleClick(int mouse_x, int mouse_y) {
        renderer_.HandleClick(mouse_x, mouse_y, const_cast<Game&>(game_));
    }

    RendererType& GetRenderer() {
        return renderer_;
    }

    const RendererType& GetRenderer() const {
        return renderer_;
    }

private:
    void OnStateChanged(GameState new_state) {
        // skip
    }

    RendererType& renderer_;
    const Game& game_;
    GameState last_state_;
};

#endif // GAME_VIEW_H
