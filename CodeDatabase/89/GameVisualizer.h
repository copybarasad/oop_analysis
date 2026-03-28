#ifndef GAME_VISUALIZER_H
#define GAME_VISUALIZER_H

#include "Renderer.h"
#include "GameManager.h"
#include <memory>

template<typename RendererType>
class GameVisualizer {
public:
    GameVisualizer() : renderer(std::make_unique<RendererType>()) {}

    void renderGame(const GameManager& gameManager) {
        renderer->render(gameManager.getGridRef(), 
                        gameManager.getPlayer(),
                        gameManager.getEnemyCount(),
                        gameManager.getTowers().size());
    }

    void renderMainMenu() {
        renderer->renderMainMenu();
    }

    void renderGameOver() {
        renderer->renderGameOver();
    }

    void renderUpgradeMenu(const Player& player) {
        renderer->renderUpgradeMenu(player);
    }

    void renderSpells(const Player& player) {
        renderer->renderSpells(player);
    }

private:
    std::unique_ptr<RendererType> renderer;
};

#endif