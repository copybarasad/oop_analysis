#include "../include/GameRenderer.h"
#include "../include/SpellType.h"
#include "../include/KeyBindingConfig.h"
#include <iostream>
#include <cctype>

void GameRenderer::renderTurnInfo(const Player& player, int turnNumber, bool isSlowed,
                                  int lastDamageDealt, int lastEnemiesKilled,
                                  const SpellHand& spellHand, const EnhancementBuffer& buffer) const {
    std::cout << "\n==========================================" << std::endl;
    std::cout << "Turn: " << turnNumber << std::endl;
    std::cout << "Player Health: " << player.getHealth() << std::endl;
    std::cout << "Player Score: " << player.getScore() << std::endl;
    std::cout << "Combat Mode: " << (player.getCombatMode() == CombatMode::MELEE ? "MELEE" : "RANGED") << std::endl;
    std::cout << "Current Damage: " << player.getCurrentDamage() << std::endl;
    
    int totalEnhancements = buffer.getTotalEnhancementCount();
    if (totalEnhancements > 0) {
        std::cout << "Enhancements: ";
        std::cout << "Fireball+" << buffer.getEnhancementCount(SpellType::DIRECT_DAMAGE) << " ";
        std::cout << "Meteor+" << buffer.getEnhancementCount(SpellType::AREA_DAMAGE) << " ";
        std::cout << "Trap+" << buffer.getEnhancementCount(SpellType::TRAP) << " ";
        std::cout << "Summon+" << buffer.getEnhancementCount(SpellType::SUMMON) << std::endl;
    }
    
    std::cout << "\nSpells in Hand:" << std::endl;
    for (int i = 0; i < spellHand.getSpellCount(); i++) {
        const Spell* spell = spellHand.getSpell(i);
        std::cout << "  [" << i << "] " << spell->getName() << " - " << spell->getDescription(buffer) << std::endl;
    }
    
    if (lastDamageDealt > 0) {
        std::cout << "\nLast Turn: Dealt " << lastDamageDealt << " total damage";
        if (lastEnemiesKilled > 0) {
            std::cout << ", killed " << lastEnemiesKilled << " enemies";
        }
        std::cout << std::endl;
    }
    
    if (isSlowed) {
        std::cout << "STATUS: SLOWED (cannot move this turn)" << std::endl;
    }
    
    std::cout << "\nLegend: P=Player A=Ally E=Enemy B=Building T=Tower ^=Trap #=Wall ~=Slowing ." << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << std::flush;
}

void GameRenderer::renderField(int width, int height, 
                               char (*getCellSymbol)(int x, int y, void* context),
                               void* context) const {
    std::cout << "\n   ";
    for (int x = 0; x < width; ++x) {
        std::cout << (x + 1) % 10 << " ";
    }
    std::cout << std::endl;
    
    for (int y = 0; y < height; ++y) {
        std::cout << (y + 1) % 10 << "  ";
        for (int x = 0; x < width; ++x) {
            std::cout << getCellSymbol(x, y, context) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::flush;
}

void GameRenderer::renderMessage(const std::string& message) const {
    std::cout << message << "\n";
}

void GameRenderer::renderGameOver(int score, int turns) const {
    std::cout << "\n=== GAME OVER ===\n";
    std::cout << "You have been defeated!\n";
    std::cout << "Final Score: " << score << "\n";
    std::cout << "Turns Survived: " << turns << "\n";
}

void GameRenderer::renderVictory(int score, int turns) const {
    std::cout << "\n=== VICTORY ===\n";
    std::cout << "You reached 1000 points!\n";
    std::cout << "Final Score: " << score << "\n";
    std::cout << "Turns Taken: " << turns << "\n";
}

void GameRenderer::renderWelcome() const {
    std::cout << "\n=== TACTICAL COMBAT GAME ===\n";
    std::cout << "Objective: Score points by defeating enemies!\n";
    std::cout << "Each enemy gives 25 points. Buildings (B) spawn new enemies.\n\n";
    
    const auto& config = KeyBindingConfig::getInstance();
    
    char moveUp = 'w', moveDown = 's', moveLeft = 'a', moveRight = 'd';
    auto bindings = config.getAllBindings();
    for (const auto& [key, desc] : bindings) {
        if (desc.find("MOVE UP") != std::string::npos) moveUp = key;
        else if (desc.find("MOVE DOWN") != std::string::npos) moveDown = key;
        else if (desc.find("MOVE LEFT") != std::string::npos) moveLeft = key;
        else if (desc.find("MOVE RIGHT") != std::string::npos) moveRight = key;
    }
    
    char attackKey = config.getKeyForCommand(CommandType::ATTACK);
    char modeKey = config.getKeyForCommand(CommandType::SWITCH_MODE);
    char castKey = config.getKeyForCommand(CommandType::CAST_SPELL);
    char buyKey = config.getKeyForCommand(CommandType::BUY_SPELL);
    char skipKey = config.getKeyForCommand(CommandType::SKIP_TURN);
    char restartKey = config.getKeyForCommand(CommandType::RESTART);
    char quitKey = config.getKeyForCommand(CommandType::QUIT);
    
    std::cout << "=== CONTROLS ===\n";
    std::cout << "Movement: " << (char)toupper(moveUp) << "/" 
              << (char)toupper(moveLeft) << "/" 
              << (char)toupper(moveDown) << "/" 
              << (char)toupper(moveRight) << "\n";
    std::cout << (char)toupper(attackKey) << " + direction - Attack\n";
    std::cout << (char)toupper(modeKey) << " - Switch combat mode (melee/ranged)\n";
    std::cout << (char)toupper(castKey) << " - Cast spell\n";
    std::cout << (char)toupper(buyKey) << " - Buy spell (50 points)\n";
    if (skipKey == ' ') {
        std::cout << "SPACE - Skip turn\n";
    } else {
        std::cout << (char)toupper(skipKey) << " - Skip turn\n";
    }
    std::cout << "SAVE - Save game\n";
    std::cout << "LOAD - Load game\n";
    std::cout << (char)toupper(restartKey) << " - Restart game\n";
    std::cout << (char)toupper(quitKey) << " - Quit\n";
    std::cout << "================\n\n";
    std::cout << std::flush;
}
