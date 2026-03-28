#ifndef GAME_VISUALIZER_H
#define GAME_VISUALIZER_H

#include <string>

template <typename TRenderer>
class GameVisualizer {
private:
    TRenderer renderer_;

public:
    template <typename GameType>
    void update(const GameType& game, const std::string& help_text) {
        renderer_.render(game, help_text);
    }
};

#endif