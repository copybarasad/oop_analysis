#ifndef GAMELB1_GAMELAUNCHER_H
#define GAMELB1_GAMELAUNCHER_H

#include "Game.h"
#include "Command.h"
#include <memory>


template <typename InputReaderT>
class GameController {
public:
    GameController(Game& game, InputReaderT& inputReader)
            : game_(game), inputReader_(inputReader) {}

    void handleInput(const sf::Event& event) {
        std::unique_ptr<Command> command = inputReader_.handleInput(event);
        if (command) {
            if (game_.getGameState() == GameState::UpgradeMenu) {
                auto* spellCmd = dynamic_cast<SelectSpellCommand*>(command.get());
                if (spellCmd) {
                }
            }
            command->execute(game_);
        }
    }

private:
    Game& game_;
    InputReaderT& inputReader_;
};

template <typename RendererT>
class GameVisualizer {
public:
    GameVisualizer(Game& game, RendererT& renderer)
            : game_(game), renderer_(renderer) {}

    void render(sf::RenderWindow& window) {
        renderer_.draw(window);
    }

private:
    Game& game_;
    RendererT& renderer_;
};

#endif //GAMELB1_GAMELAUNCHER_H