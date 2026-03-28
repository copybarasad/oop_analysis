#include "../include/GameVisualizer.h"
#include "../include/Game.h"
#include <iostream>
#include <iomanip>

void ConsoleRenderer::renderGame(const Game& game) {
    std::cout << "\033[2J\033[H";
    
    renderField(game);
    renderStatus(game);
    renderSpells(game);
}

void ConsoleRenderer::renderField(const Game& game) const {
    int width = game.getFieldSize();
    int height = game.getFieldSize();
    
    std::cout << "+";
    for (int x = 0; x < width; x++) {
        std::cout << "-";
    }
    std::cout << "+\n";
    
    for (int y = 0; y < height; y++) {
        std::cout << "|";
        for (int x = 0; x < width; x++) {
            std::cout << game.getCellSymbol(x, y);
        }
        std::cout << "|\n";
    }
    
    std::cout << "+";
    for (int x = 0; x < width; x++) {
        std::cout << "-";
    }
    std::cout << "+\n";
}

void ConsoleRenderer::renderStatus(const Game& game) const {
    const Player& player = game.getPlayer();
    
    std::cout << "\n=== Turn " << game.getTurnNumber() << " ===\n";
    std::cout << "HP: " << player.getHealth() << "/" << player.getMaxHealth();
    std::cout << " | Score: " << player.getScore();
    std::cout << " | Mode: " << (player.getCombatMode() == CombatMode::MELEE ? "MELEE" : "RANGED");
    std::cout << " | Level: " << game.getCurrentLevel();
    std::cout << "\n";
    
    if (player.getIsSlowed()) {
        std::cout << "[SLOWED - 2 actions next turn]\n";
    }
}

void ConsoleRenderer::renderSpells(const Game& game) const {
    const SpellHand& spellHand = game.getSpellHand();
    const EnhancementBuffer& buffer = game.getEnhancementBuffer();
    
    std::cout << "\n=== Spells (" << spellHand.getSpellCount() << "/5) ===\n";
    
    for (int i = 0; i < spellHand.getSpellCount(); i++) {
        const Spell* spell = spellHand.getSpell(i);
        if (spell) {
            std::cout << (i + 1) << ". " << spell->getName() 
                     << " - " << spell->getDescription(buffer) << "\n";
        }
    }
    
    if (spellHand.getSpellCount() < 5) {
        std::cout << "(Press 'b' to buy spell for 50 points)\n";
    }
}

void ConsoleRenderer::renderMessage(const std::string& message) const {
    std::cout << "\n" << message << "\n";
}

void ConsoleRenderer::renderGameOver(int score, int turns) const {
    std::cout << "\n";
    std::cout << "╔════════════════════════════════╗\n";
    std::cout << "║         GAME OVER              ║\n";
    std::cout << "╠════════════════════════════════╣\n";
    std::cout << "║  Final Score: " << std::setw(5) << score << "            ║\n";
    std::cout << "║  Turns Played: " << std::setw(4) << turns << "           ║\n";
    std::cout << "╚════════════════════════════════╝\n";
}

void ConsoleRenderer::renderVictory(int score, int turns) const {
    std::cout << "\n";
    std::cout << "╔════════════════════════════════╗\n";
    std::cout << "║       VICTORY!                 ║\n";
    std::cout << "╠════════════════════════════════╣\n";
    std::cout << "║  Final Score: " << std::setw(5) << score << "            ║\n";
    std::cout << "║  Turns Played: " << std::setw(4) << turns << "           ║\n";
    std::cout << "╚════════════════════════════════╝\n";
}

void ConsoleRenderer::renderControls() const {
    const auto& config = KeyBindingConfig::getInstance();
    
    std::cout << "\n=== CONTROLS ===\n";
    
    char moveUp = config.getKeyForCommand(CommandType::MOVE);
    char moveDown = 's', moveLeft = 'a', moveRight = 'd';
    
    auto bindings = config.getAllBindings();
    for (const auto& [key, desc] : bindings) {
        if (desc.find("MOVE UP") != std::string::npos) moveUp = key;
        else if (desc.find("MOVE DOWN") != std::string::npos) moveDown = key;
        else if (desc.find("MOVE LEFT") != std::string::npos) moveLeft = key;
        else if (desc.find("MOVE RIGHT") != std::string::npos) moveRight = key;
    }
    
    std::cout << "Movement: " << (char)toupper(moveUp) << "/" 
              << (char)toupper(moveLeft) << "/" 
              << (char)toupper(moveDown) << "/" 
              << (char)toupper(moveRight) << "\n";
    
    char attackKey = config.getKeyForCommand(CommandType::ATTACK);
    char modeKey = config.getKeyForCommand(CommandType::SWITCH_MODE);
    char castKey = config.getKeyForCommand(CommandType::CAST_SPELL);
    char buyKey = config.getKeyForCommand(CommandType::BUY_SPELL);
    char skipKey = config.getKeyForCommand(CommandType::SKIP_TURN);
    char restartKey = config.getKeyForCommand(CommandType::RESTART);
    char quitKey = config.getKeyForCommand(CommandType::QUIT);
    
    std::cout << (char)toupper(attackKey) << " + direction - Attack\n";
    std::cout << (char)toupper(modeKey) << " - Switch combat mode (melee/ranged)\n";
    std::cout << (char)toupper(castKey) << " - Cast spell\n";
    std::cout << (char)toupper(buyKey) << " - Buy spell (50 points)\n";
    if (skipKey == ' ') {
        std::cout << "SPACE - Skip turn\n";
    } else {
        std::cout << (char)toupper(skipKey) << " - Skip turn\n";
    }
    std::cout << (char)toupper(restartKey) << " - Restart game\n";
    std::cout << (char)toupper(quitKey) << " - Quit\n";
    std::cout << "================\n\n";
}

void ConsoleRenderer::renderWelcome() const {
    std::cout << "\n";
    std::cout << "╔════════════════════════════════╗\n";
    std::cout << "║   TACTICAL COMBAT GAME         ║\n";
    std::cout << "╚════════════════════════════════╝\n";
    renderControls();
}

void ConsoleRenderer::onGameEvent(const GameEvent& event) {
    switch (event.type) {
        case GameEventType::PLAYER_DAMAGED:
            std::cout << "You took " << event.value1 << " damage!\n";
            break;
        case GameEventType::ENEMY_KILLED:
            std::cout << "Enemy killed! +" << event.value1 << " points\n";
            break;
        case GameEventType::LEVEL_COMPLETED:
            std::cout << "Level " << event.value1 << " completed!\n";
            break;
        case GameEventType::PLAYER_SPELL_CAST:
            std::cout << "Spell cast successfully!\n";
            break;
        default:
            break;
    }
}
