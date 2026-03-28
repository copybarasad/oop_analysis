#include "gameview.hpp"

template<>
void GameView<ConsoleRenderer>::render() {
    if (!game) {
        std::cerr << "AHHHH GameView: No game instance set!\n";
        return;
    }
    renderer.render(*game);
}

template<>
void GameView<ConsoleRenderer>::renderMessage(const std::string& message) {
    renderer.renderMessage(message);
}