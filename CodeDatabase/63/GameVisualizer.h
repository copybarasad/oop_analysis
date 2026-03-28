#ifndef GAME_VISUALIZER_H
#define GAME_VISUALIZER_H

#include "Game.h"
#include "Player.h"

template<typename Renderer>
class GameVisualizer {
    Renderer renderer;
    std::shared_ptr<Game>& game;
public:
    explicit GameVisualizer(std::shared_ptr<Game>& game) : game(game) {}

    void visualize(const std::string& message, bool alert) {
        if (alert || !game) {
            renderer.showMessage(message);
            return;
        }
        auto& player = game->getPlayer();
        auto mode = player.getMode();
        if (mode == PlayerModes::SHOP) {
            renderer.showMessage(game->getShop().getInfo(player));
        } else if (mode == PlayerModes::UPGRADE) {
            renderer.showMessage(player.upgradeInfo());
        } else {
            renderer.showGame(game, message);
        }
    }
};

#endif
