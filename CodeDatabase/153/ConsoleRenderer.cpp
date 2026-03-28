#include "ConsoleRenderer.h"
#include "GameState.h"
#include "CellContent.h"
#include <iostream>

void ConsoleRenderer::updateFieldContent(GameField& field, const GameState& gameState) const {
    field.clearAllCells();

    const auto& enemies = gameState.getEnemies();
    for (const auto& enemy : enemies) {
        if (enemy.isAlive()) {
            field.setCellContent(enemy.getX(), enemy.getY(), CellContent::ENEMY);
        }
    }

    auto playerPos = field.getPlayerPosition();
    field.setCellContent(playerPos.first, playerPos.second, CellContent::PLAYER);
}

std::string ConsoleRenderer::getGameStateString(const GameState& gameState) const {
    std::stringstream ss;

    const auto& player = gameState.getPlayer();
    auto playerPos = gameState.getField().getPlayerPosition();

    ss << "Health: " << player.getHealth();
    ss << " | Mana: " << player.getMana() << "/" << player.getMaxMana();
    ss << " | Score: " << player.getScore();
    ss << " | Position: (" << playerPos.first << "," << playerPos.second << ")\n\n";

    auto& field = const_cast<GameField&>(gameState.getField());
    updateFieldContent(field, gameState);

    for (int y = 0; y < field.getHeight(); y++) {
        for (int x = 0; x < field.getWidth(); x++) {
            ss << field.getCellContent(x, y) << " ";
        }
        ss << "\n";
    }

    ss << "\n=== Spells ===\n";
    const auto& spellHand = player.getSpellHand();
    if (spellHand.getSpellCount() > 0) {
        for (int i = 0; i < spellHand.getSpellCount(); i++) {
            ss << i + 1 << ". " << spellHand.getSpellDescription(i) << "\n";
        }
    }
    else {
        ss << "No spells\n";
    }

    ss << "\n=== Enemies ===\n";
    const auto& enemies = gameState.getEnemies();
    int aliveEnemies = 0;
    for (const auto& enemy : enemies) {
        if (enemy.isAlive()) {
            aliveEnemies++;
            ss << "Enemy at (" << enemy.getX() << "," << enemy.getY()
                << ") HP: " << enemy.getHealth() << "\n";
        }
    }

    if (aliveEnemies == 0) {
        ss << "No enemies\n";
    }

    return ss.str();
}

void ConsoleRenderer::render(const GameState& gameState) const {
    std::string gameStateStr = getGameStateString(gameState);
    std::cout << gameStateStr << std::endl;
}

void ConsoleRenderer::showMessage(const std::string& message) const {
    std::cout << message << std::endl;
}