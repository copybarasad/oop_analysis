#include "console_render.h"
#include "game_constants.h"

void ConsoleRenderer::render(const GameController& game) const {
    std::cout << "=== Game State ===\n";
    renderStats(game);
    std::cout << "\n";
    renderField(game);
    std::cout << "\n";
}

void ConsoleRenderer::renderStats(const GameController& game) const {
    std::cout << "Player - Health: " << game.getPlayer().getHealth()
        << " | Score: " << game.getPlayer().getScore()
        << " | Level: " << game.getCurrentLevel()
        << " | Enemies alive: "
        << std::count_if(game.getEnemies().begin(), game.getEnemies().end(),
            [](const Enemy& e) { return e.isAlive(); })
        << "\n";
}

void ConsoleRenderer::renderField(const GameController& game) const {
    const auto& field = game.getGameField();
    for (int y = 0; y < field.getHeight(); ++y) {
        for (int x = 0; x < field.getWidth(); ++x) {
            Position pos(x, y);
            CellType type = field.getCellType(pos);
            char symbol = '.';
            if (type == CellType::PLAYER) symbol = 'P';
            else if (type == CellType::ENEMY) symbol = 'E';
            std::cout << symbol << ' ';
        }
        std::cout << '\n';
    }
}

void ConsoleRenderer::renderMainMenu() const {
    std::cout << "=== ROGUELIKE ===\n";
    std::cout << "[N] New Game\n";
    std::cout << "[L] Load Game\n";
    std::cout << "[Q] Quit\n> ";
}

void ConsoleRenderer::renderGameOver(const GameController& game) const {
    std::cout << "Game Over! Player died.\n";
    std::cout << "Final Score: " << game.getPlayer().getScore() << "\n";
    std::cout << "(r)estart, (l)oad, (q)uit: ";
}

void ConsoleRenderer::renderLevelUp(int level) const {
    std::cout << "================================\n";
    std::cout << " LEVEL PASSED! LEVEL " << level << " \n";
    std::cout << "================================\n\n";
}

void ConsoleRenderer::renderShop(const GameController& game) const {
    const auto& shop = game.getSpellShop();
    const auto& offers = shop.getAvailableSpells();
    std::cout << "=== Spell Shop ===\n";
    std::cout << "Your score: " << game.getPlayer().getScore() << " points\n";
    std::cout << "Hand: " << game.getPlayer().getHand().getSize() << "/" << game.getPlayer().getHand().getMaxSize() << "\n";
    std::cout << "Spells are ONE-TIME USE only!\n\n";

    if (offers.empty()) {
        std::cout << "No spells available.\n";
        return;
    }

    for (size_t i = 0; i < offers.size(); ++i) {
        std::cout << i + 1 << ". " << offers[i].name << " - " << offers[i].cost << " points\n";
        std::cout << "  " << offers[i].description << "\n";
        std::cout << "  [ONE-TIME USE]\n";
        if (game.getPlayer().canAffordSpell(offers[i].cost) && game.getPlayer().canLearnSpell()) {
            std::cout << "  [CAN BUY]\n";
        }
        else if (!game.getPlayer().canAffordSpell(offers[i].cost)) {
            std::cout << "  [NEED " << (offers[i].cost - game.getPlayer().getScore()) << " MORE POINTS]\n";
        }
        else {
            std::cout << "  [HAND FULL]\n";
        }
        std::cout << "\n";
    }
}

void ConsoleRenderer::renderSpells(const GameController& game) const {
    const auto& hand = game.getPlayer().getHand();
    std::cout << "Available spells:\n";
    for (size_t i = 0; i < hand.getSize(); ++i) {
        auto* spell = hand.getSpell(i);
        std::string status = spell->isUsed() ? "[USED] " : "[READY] ";
        std::cout << i + 1 << ". " << status << spell->getName()
            << " - " << spell->getDescription() << "\n";
    }
}

void ConsoleRenderer::renderTargetHelp(const GameController& game, const std::vector<Position>& validTargets, bool isArea) const {
    std::cout << (isArea ? "Valid area targets (marked with #):\n" : "Valid targets (marked with *):\n");
    const auto& field = game.getGameField();
    for (int y = 0; y < field.getHeight(); ++y) {
        for (int x = 0; x < field.getWidth(); ++x) {
            Position pos(x, y);
            bool isValid = std::find(validTargets.begin(), validTargets.end(), pos) != validTargets.end();
            char symbol = isValid ? (isArea ? '#' : '*') : '.';
            if (game.getGameField().getCellType(pos) == CellType::PLAYER) symbol = 'P';
            else if (game.getGameField().getCellType(pos) == CellType::ENEMY) symbol = 'E';
            std::cout << symbol << ' ';
        }
        std::cout << '\n';
    }
    std::cout << "Legend: P=Player, E=Enemy, " << (isArea ? '#' : '*') << "=valid target\n";
}

void ConsoleRenderer::renderQuitPrompt() const {
    std::cout << "Quit? Save first? (y/n/s): ";
}