#include "ConsoleRenderer.h"
#include "GameController.h"
#include "Logger.h"
#include <iostream>

void ConsoleRenderer::render(const GameController& game) {
    Logger::clearScreen();
    std::cout << "Level: " << game.getCurrentLevel() << std::endl;
    game.getField().print();

    const auto& player = game.getPlayer();
    std::cout << "======================================================" << std::endl;
    std::cout << "HP: " << player.getHealth()
              << " | Score: " << player.getScore()
              << " | Mode: " << player.getModeString()
              << " (Dmg: " << player.getDamage() << ")" << std::endl;
    std::cout << "======================================================" << std::endl;
    if (player.getHand()) {
        player.getHand()->displaySpells();
    }
    Logger::displayMessages();
}