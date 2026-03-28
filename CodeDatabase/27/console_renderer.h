#pragma once

#include <ostream>

class Game;

class ConsoleRenderer {
public:
    ConsoleRenderer();
    explicit ConsoleRenderer(std::ostream& output);

    void Render(const Game& game);

private:
    std::ostream* out;
};
