#include <ncurses.h>
#include <Field.hpp>
#include <Game.hpp>
#include <clocale>

int main() {
    setlocale(LC_ALL, "");
    Game game;
    if (game.help()) {
        game.start();
    }
    return 0;
}