#pragma once
#include <string>
#include "gamemodel.hpp"
#include "cell.hpp"
#include "event_dispatcher.hpp"
#include "events.hpp"

class ConsoleRenderer {
public:
    explicit ConsoleRenderer(Game& game);

    void DrawMessage(const std::string& msg);
    void DrawField();

private:
    void DrawCell(CellType type);
    void Subscribe();
    Game* game_;
};
