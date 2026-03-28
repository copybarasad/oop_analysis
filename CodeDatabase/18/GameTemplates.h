#pragma once
#include "GameLogic.h"

template <typename RendererT>
class GameVisualizer {
private:
    RendererT renderer;

public:
    RendererT& getRenderer() { return renderer; }

    void draw(const GameLogic& game, int spellIdx) {
        renderer.render(game, spellIdx);
    }

    bool isOpen() { return renderer.isOpen(); }
};

template <typename InputHandlerT>
class GameController {
private:
    InputHandlerT inputHandler;
    GameLogic& gameLogic;

public:
    explicit GameController(GameLogic& logic) : gameLogic(logic) {}

    InputHandlerT& getInput() { return inputHandler; }

    void processEvents(sf::Window& window) {
        inputHandler.setWindow(window);
    }

    void handleEvent(const sf::Event& event) {
        auto cmd = inputHandler.handleInput(event);
        if (cmd) cmd->execute(gameLogic);

        auto mouseCmd = inputHandler.handleMouse(event, 27.f);
        if (mouseCmd) mouseCmd->execute(gameLogic);
    }

    int getSelectedSpellIndex() const {
        return inputHandler.getSelectedSpellIndex();
    }
};