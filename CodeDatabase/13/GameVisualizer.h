#ifndef GAME_GAMEVISUALIZER_H
#define GAME_GAMEVISUALIZER_H
#include "GameLogic.h"
#include <SFML/Graphics.hpp>


template<typename Render>
class GameVisualizer {
public:
    explicit GameVisualizer(GameLogic *glogic)
        : renderer_(glogic) {
    }

    void run() { renderer_.run(); }
    void close() { renderer_.close(); }
    bool isOpen() { return renderer_.isOpen(); }
    void renderGame() { renderer_.renderGame(); }
    int renderMenu(const std::vector<std::string> &options) { return renderer_.renderMenu(options); }
    void switchOver(bool sw) { renderer_.switchOver(sw); }
    void switchSpellInfoState() { return renderer_.switchSpellInfoState(); };
    bool pollEvent(sf::Event &e) { return renderer_.pollEvent(e); }

private:
    Render renderer_;
};
#endif
