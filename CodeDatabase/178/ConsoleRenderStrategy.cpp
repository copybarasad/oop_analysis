#include "ConsoleRenderStrategy.h"
#include "GameController.h"
#include "RenderSystem.h"
#include <iostream>
#ifdef _WIN32
#include <windows.h>
#else
#include <cstdlib>
#endif

void ConsoleRenderStrategy::renderGame(const GameController& controller) {
    RenderSystem renderSystem(controller.getField(), controller.getPlayer(),
                             controller.getEnemies(), controller.getBuildings(),
                             controller.getTraps(), controller.getAllies(),
                             controller.getTowers());
    renderSystem.renderGame();
}

void ConsoleRenderStrategy::renderMenu(const std::string& menuText) {
    printSeparator();
    std::cout << menuText << std::endl;
    printSeparator();
}

void ConsoleRenderStrategy::renderMessage(const std::string& message) {
    std::cout << message << std::endl;
}

void ConsoleRenderStrategy::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void ConsoleRenderStrategy::printSeparator() const {
    std::cout << "==================================" << std::endl;
}