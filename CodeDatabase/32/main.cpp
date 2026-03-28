#include "game_controller.hpp"
#include <ctime>
#include <cstdlib>

int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    GameController controller;
    controller.Run();

    return 0;
}