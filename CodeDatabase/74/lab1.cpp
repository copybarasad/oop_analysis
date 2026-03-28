#include "GameManager.h"
#include "Logger.h"

int main() {
    Logger::init("game_log.txt");

    GameManager manager;
    manager.run();

    return 0;
}
