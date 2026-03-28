// main.cpp
#include "Control.h"
#include "FullGameInput.h"
#include "Render.h"
#include "Game.h"
#include "LevelManager.h"

int main() {
    GameConfig cfg;
    Game game(cfg);
    LevelManager lev;
    FullGameInput input;  // ← должен иметь метод std::optional<Command> getNextCommand()

    Control<FullGameInput, ConsoleRenderer> ctrl(game, input, lev);
    ctrl.start();
    return 0;  // ← как в оригинале: сразу 1-й уровень
}