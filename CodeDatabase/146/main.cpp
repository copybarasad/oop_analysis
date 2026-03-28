#include "GameWrappers.h"
#include "InputReader.h"
#include "ConsoleRenderer.h"
#include "ConsoleView.h"
#include "Game.h"
#include <ctime>

int main() {
    srand(static_cast<unsigned>(time(0)));

    Game game;
    ConsoleView view;
    game.set_view(&view);
    GameManager<InputReader, GameVisualizer<ConsoleRenderer>> app;
    app.run(game);

    return 0;
}
