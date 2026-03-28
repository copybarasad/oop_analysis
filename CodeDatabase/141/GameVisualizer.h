#pragma once

#include "RenderSystem.h"
#include "Command.h"

template<typename Renderer>
class GameVisualizer {
public:
    explicit GameVisualizer(Renderer &renderer) : renderer(renderer) {
    }

    template<typename GameType>
    void render(const GameType &game) {
        renderer.render(game.getField(), game.getPlayer(), game.getEnemies(),
                        game.getBuildings(), game.getTowers(), game.getTraps(),
                        game.getAllies(), game.getEnemiesKilled(),
                        game.getScoreTarget(), game.getCurrentLevel());
    }

    template<typename GameType>
    void promptSpellSelection(const GameType &game) const {
        renderer.displaySpellSelection(game.getPlayer().getHand());
    }

    void promptInput(const std::string &text) const {
        renderer.promptInput(text);
    }

    void promptSaveSlot() const {
        renderer.addMessage("Сохранение...");
    }

    void promptLoadSlot() const {
        renderer.addMessage("Загрузка...");
    }

    void showMessage(const std::string &text) const {
        renderer.addMessage(text);
    }

private:
    Renderer &renderer;
};
