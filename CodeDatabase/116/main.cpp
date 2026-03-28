#include "GameEngine.h"
#include <iostream>

int main() {
    GameEngine engine;
    engine.run();
    std::cout << "Игра окончена!" << std::endl;
    return 0;
}