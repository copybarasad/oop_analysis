#include "Renderer.h"
#include "Player.h"
#include "SpellHand.h"
#include "AreaDamageSpell.h"
#include "DirectDamageSpell.h"
#include "Field.h"
#include <iostream>



void GameRenderer::renderPlayerSpellInfo(const Player& player) const {
    if (player.getSpellHand().getSpellCount() == 0) {
        std::cout << "No spells available.\n";
        return;
    }
    
    std::cout << "=== Spells ===\n";
    std::cout << "Mana: " << player.getMana() << "/" << player.getMaxMana() << "\n";
    std::cout << "Spells in hand (" << player.getSpellHand().getSpellCount() 
              << "/" << player.getSpellHand().getMaxSize() << "):\n";
    
    for (size_t i = 0; i < player.getSpellHand().getSpellCount(); ++i) {
        const Spell* spell = player.getSpellHand().getSpell(i);
        if (spell) {
            std::cout << "  " << i + 1 << ". " << spell->getName() 
                      << " (Cost: " << spell->getManaCost() << " mana) - "
                      << spell->getDescription() << "\n";
        }
    }
}



void GameRenderer::renderSpellCastSuccess(int remainingMana) const {
    std::cout << "Spell cast successfully! Remaining mana: " << remainingMana << "\n";
}


void GameRenderer::renderSpellInfo(const Player& player) const {
    std::cout << "Mana: " << player.getMana() << "/" << player.getMaxMana() << std::endl;
    std::cout << "Spells in hand: " << player.getSpellHand().getSpellCount() 
              << "/" << player.getSpellHand().getMaxSize() << std::endl;
    
    for (size_t i = 0; i < player.getSpellHand().getSpellCount(); ++i) {
        const Spell* spell = player.getSpellHand().getSpell(i);
        if (spell) {
            std::cout << i + 1 << ". " << spell->getName() 
                      << " (" << spell->getManaCost() << " mana) - " 
                      << spell->getDescription();
            
            if (!player.canCastSpell(*spell)) {
                std::cout << " [Not enough mana]";
            }
            std::cout << std::endl;
        }
    }
}

void GameRenderer::renderSpellPurchased() const {
    std::cout << "Spell purchased successfully!" << std::endl;
}

void GameRenderer::renderSpellHandFull() const {
    std::cout << "Spell hand is full! Cannot purchase new spell." << std::endl;
}


void GameRenderer::renderPlayerAttackEnemy(int damage, int currentHealth, int maxHealth) const {
    std::cout << "Player attacks enemy for " << damage << " damage! ";
    std::cout << "Enemy health: " << currentHealth << "/" << maxHealth << "\n";
}

void GameRenderer::renderEnemyDefeated(int scoreBonus) const {
    std::cout << "Enemy defeated! +" << scoreBonus << " score\n";
}

void GameRenderer::renderEnemyAttacksPlayer(const Enemy& enemy, int damage) const {
    std::cout << "Enemy attacks you for " << damage << " damage!\n";
}

void GameRenderer::renderLevelTransition(int level, int width, int height, int nextLevelScore) const {
    std::cout << "\n=== LEVEL " << level << " UNLOCKED! ===\n";
    std::cout << "Player health restored!\n";
    std::cout << "New field size: " << width << "x" << height << "\n";
    std::cout << "Score needed for next level: " << nextLevelScore << "\n";
}

void GameRenderer::renderSpellLossInfo(int spellsLost) const {
    if (spellsLost == 0) {
        std::cout << "You managed to keep your spell!\n";
    } else if (spellsLost == 1) {
        std::cout << "One spell was lost during level transition!\n";
    } else {
        std::cout << spellsLost << " spells were lost during level transition!\n";
    }
}

void GameRenderer::renderLevelUpMenu(const Player& player) const {
    std::cout << "\n LEVEL UP! Choose your upgrade\n";
    std::cout << "1. +25 Max Health (Current: " << player.getMaxHealth() << ")\n";
    std::cout << "2. +20 Max Mana (Current: " << player.getMaxMana() << ")\n";
    std::cout << "3. +5 Melee Damage (Current: " << player.getMeleeDamage() << ")\n";
    std::cout << "4. +5 Ranged Damage (Current: " << player.getRangedDamage() << ")\n";
    std::cout << "5. Upgrade Spell\n";
    std::cout << "Choice (1-5): ";
}

void GameRenderer::renderSpellUpgradeMenu(const Player& player) const {
    const SpellHand& spellHand = player.getSpellHand();
    
    if (spellHand.getSpellCount() == 0) {
        std::cout << "No spells to upgrade! Health upgraded instead.\n";
        return;
    }
    
    std::cout << "\nChoose spell to upgrade:\n";
    for (int i = 0; i < spellHand.getSpellCount(); ++i) {
        Spell* spell = spellHand.getSpell(i);
        std::cout << i + 1 << ". " << spell->getName() << " - " << spell->getDescription() << "\n";
        
        DirectDamageSpell* directSpell = dynamic_cast<DirectDamageSpell*>(spell);
        AreaDamageSpell* areaSpell = dynamic_cast<AreaDamageSpell*>(spell);
        
        if (directSpell) {
            std::cout << "   Damage: " << directSpell->getCurrentDamage() 
                      << ", Range: " << directSpell->getCurrentRange() << "\n";
        } else if (areaSpell) {
            std::cout << "   Damage: " << areaSpell->getCurrentDamage() 
                      << ", Area: " << areaSpell->getCurrentAreaSize() << "x" 
                      << areaSpell->getCurrentAreaSize() << "\n";
        }
    }
    
    std::cout << "Choice (1-" << spellHand.getSpellCount() << "): ";
}

void GameRenderer::renderSpellUpgradeOptions(const Spell* spell) const {
    if (!spell) return;
    
    const DirectDamageSpell* directSpell = dynamic_cast<const DirectDamageSpell*>(spell);
    const AreaDamageSpell* areaSpell = dynamic_cast<const AreaDamageSpell*>(spell);
    
    if (directSpell) {
        std::cout << "\nChoose upgrade for " << directSpell->getName() << ":\n";
        std::cout << "1. +10 Damage (Current: " << directSpell->getCurrentDamage() << ")\n";
        std::cout << "2. +1 Range (Current: " << directSpell->getCurrentRange() << ")\n";
        std::cout << "Choice (1-2): ";
    } else if (areaSpell) {
        std::cout << "\nChoose upgrade for " << areaSpell->getName() << ":\n";
        std::cout << "1. +8 Damage (Current: " << areaSpell->getCurrentDamage() << ")\n";
        std::cout << "2. +1 Area Size (Current: " << areaSpell->getCurrentAreaSize() << "x" 
                  << areaSpell->getCurrentAreaSize() << ")\n";
        std::cout << "Choice (1-2): ";
    } else {
        std::cout << "Unknown spell type. Health upgraded instead.\n";
    }
}

void GameRenderer::renderPlayerStats(const Player& player, int level) const {
    std::cout << "Level: " << level << " | ";
    std::cout << "Health: " << player.getHealth() << "/" << player.getMaxHealth() << " | ";
    std::cout << "Mana: " << player.getMana() << "/" << player.getMaxMana() << " | ";
    std::cout << "Score: " << player.getScore() << " | ";
    std::cout << "Combat: " << (player.getCombatType() == CombatType::MELEE ? "Melee" : "Ranged") << " | ";
    std::cout << "Can move: " << (player.canMoveNextTurn() ? "Yes" : "No") << "\n";
    std::cout << "Melee: " << player.getMeleeDamage() << " | ";
    std::cout << "Ranged: " << player.getRangedDamage() << " | ";
    std::cout << "Spells: " << player.getSpellHand().getSpellCount() << "/" 
              << player.getSpellHand().getMaxSize() << "\n\n";
}


void GameRenderer::renderGameField(const GameField& field) const {
    // Отрисовка координат X
    std::cout << "  "; 
    for (int x = 0; x < field.getWidth(); x++) {
        std::cout << x % 10 << " "; 
    }
    std::cout << "\n";
    
    // Отрисовка строк поля
    for (int y = 0; y < field.getHeight(); y++) {
        std::cout << y % 10 << " "; 
        
        for (int x = 0; x < field.getWidth(); x++) {
            if (field.hasPlayer(x, y)) {
                std::cout << "P ";
            } else if (field.hasEnemy(x, y)) {
                std::cout << "E ";
            } else if (field.hasBuilding(x, y)) {
                std::cout << "B ";
            } else {
                switch (field.getCellType(x, y)) {
                    case CellType::BLOCKED: std::cout << "# "; break;
                    case CellType::SLOW: std::cout << "~ "; break;
                    case CellType::EMPTY: std::cout << ". "; break;
                }
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

void GameRenderer::renderEnemiesSpawned(int count, int level) const {
    std::cout << "Spawned " << count << " enemies for level " << level << "\n";
}

void GameRenderer::renderSpellUpgradeDetails(const Spell* spell) const {
    if (!spell) return;
    
    const DirectDamageSpell* directSpell = dynamic_cast<const DirectDamageSpell*>(spell);
    const AreaDamageSpell* areaSpell = dynamic_cast<const AreaDamageSpell*>(spell);
    
    if (directSpell) {
        std::cout << "   Damage: " << directSpell->getCurrentDamage() 
                  << ", Range: " << directSpell->getCurrentRange() << "\n";
    } else if (areaSpell) {
        std::cout << "   Damage: " << areaSpell->getCurrentDamage() 
                  << ", Area: " << areaSpell->getCurrentAreaSize() << "x" 
                  << areaSpell->getCurrentAreaSize() << "\n";
    }
}

void GameRenderer::renderSpellSpecificUpgradeOptions(const Spell* spell) const {
    if (!spell) return;
    
    const DirectDamageSpell* directSpell = dynamic_cast<const DirectDamageSpell*>(spell);
    const AreaDamageSpell* areaSpell = dynamic_cast<const AreaDamageSpell*>(spell);
    
    if (directSpell) {
        std::cout << "\nChoose upgrade for " << directSpell->getName() << ":\n";
        std::cout << "1. +10 Damage (Current: " << directSpell->getCurrentDamage() << ")\n";
        std::cout << "2. +1 Range (Current: " << directSpell->getCurrentRange() << ")\n";
        std::cout << "Choice (1-2): ";
    } else if (areaSpell) {
        std::cout << "\nChoose upgrade for " << areaSpell->getName() << ":\n";
        std::cout << "1. +8 Damage (Current: " << areaSpell->getCurrentDamage() << ")\n";
        std::cout << "2. +1 Area Size (Current: " << areaSpell->getCurrentAreaSize() << "x" 
                  << areaSpell->getCurrentAreaSize() << ")\n";
        std::cout << "Choice (1-2): ";
    } else {
        std::cout << "Unknown spell type. Health upgraded instead.\n";
    }
}

void GameRenderer::renderNoSpellsToUpgrade() const {
    std::cout << "No spells to upgrade! Health upgraded instead.\n";
}

void GameRenderer::renderUnknownSpellType() const {
    std::cout << "Unknown spell type. Health upgraded instead.\n";
}

void GameRenderer::renderGameStateOperation(const std::string& operation, int level) const {
    if (operation == "save") {
        std::cout << "Game saved successfully! (Level " << level << ")\n";
    } else if (operation == "load") {
        std::cout << "Game loaded successfully! Level " << level << "\n";
    }
}

void GameRenderer::renderSpellDamageEffect(const std::string& spellType, 
                                         const std::string& result, 
                                         int x, int y, int damage) const {
    if (spellType == "direct") {
        if (result == "hit") {
            std::cout << "Direct damage spell hit enemy at (" << x << "," << y 
                      << ") for " << damage << " damage!\n";
        } else if (result == "miss") {
            std::cout << "No enemy at target location (" << x << "," << y << ")\n";
        }
    } else if (spellType == "area") {
        if (result == "hit") {
            std::cout << "Area spell hit enemy at (" << x << "," << y 
                      << ") for " << damage << " damage!\n";
        } else if (result == "miss") {
            std::cout << "Area spell cast at (" << x << "," << y 
                      << ") but no enemies in the area\n";
        }
    }
}

void GameRenderer::renderSpellTargetIssue(const std::string& issueType, 
                                        int distance, int range) const {
    if (issueType == "out_of_range") {
        std::cout << "Target out of range! Distance: " << distance << ", Range: " << range << "\n";
    } else if (issueType == "out_of_bounds") {
        std::cout << "Target coordinates out of bounds!\n";
    }
}

void GameRenderer::renderOperationResult(const std::string& operation, 
                                       const std::string& result,
                                       const std::string& details) const {
    if (operation == "choice" || operation == "command") {
        if (result == "invalid") {
            std::cout << "Invalid " << operation << ". " << details << "\n";
        }
    } else if (operation == "spell") {
        if (result == "cannot_cast") {
            std::cout << "Cannot cast spell: " << details << "\n";
        } else if (result == "failed") {
            std::cout << "Spell failed!\n";
        } else if (result == "success") {
            std::cout << "Spell cast successfully! " << details << "\n";
        }
    } else if (operation == "purchase") {
        if (result == "success") {
            std::cout << "Spell purchased successfully!\n";
        } else if (result == "hand_full") {
            std::cout << "Spell hand is full! Cannot purchase new spell.\n";
        }
    } else {
        std::cout << "[" << operation << "] " << result << ": " << details << "\n";
    }
}

void GameRenderer::renderInsufficientResource(const std::string& resourceType, 
                                            int needed, int have) const {
    if (resourceType == "mana") {
        std::cout << "Not enough mana! Need " << needed << ", have " << have << "\n";
    } else if (resourceType == "score") {
        std::cout << "Not enough score points! Need " << needed << ", but have " << have << "\n";
    }
}

void GameRenderer::renderUpgradeResult(const std::string& targetType,
                                     const std::string& upgradeType,
                                     const std::string& name,
                                     int newValue) const {
    if (targetType == "player_stat") {
        std::cout << name << " upgraded to " << newValue << "!\n";
    } else if (targetType == "spell") {
        std::cout << name << " " << upgradeType << " increased to " << newValue << "!\n";
    }
}

void GameRenderer::renderLevelStart(int level) const {
    std::cout << "\n=== LEVEL " << level << " STARTED ===\n";
    std::cout << "Destroy all enemies to advance to next level!\n";
}

void GameRenderer::renderGameOver(int finalScore) const {
    std::cout << "\n=== GAME OVER ===\n";
    std::cout << "Final Score: " << finalScore << "\n";
}

void GameRenderer::renderError(const std::string& context, const std::string& message) const {
    std::cout << "Error in " << context << ": " << message << "\n";
}

void GameRenderer::renderSpellShop(int playerScore, int spellsInHand, int maxSpells) const {
    std::cout << "SPELL SHOP\n";
    std::cout << "Your score: " << playerScore << "\n";
    std::cout << "Spells in hand: " << spellsInHand << "/" << maxSpells << "\n\n";
    
    std::cout << "Available spells (cost: 50 points each):\n";
    std::cout << "1. Fireball - Direct damage spell\n";
    std::cout << "   Deals 30 damage to single target, range: 3\n";
    std::cout << "2. Firestorm - Area damage spell\n";
    std::cout << "   Deals 25 damage in 2x2 area, range: 2\n\n";
}

void GameRenderer::renderMainMenu() const {
    std::cout << "\nChoose option:\n";
    std::cout << "1. New Game\n";
    std::cout << "2. Load Game\n";
    std::cout << "3. Exit\n";
    std::cout << "Choice: ";
}

void GameRenderer::renderSizeInputPrompt() const {
    std::cout << "Enter field width and height (10-20 each): ";
}

void GameRenderer::renderControlsHelp(const std::string& controlsHelp) const {
    std::cout << "Available commands: " << controlsHelp << "\n";
    std::cout << "Command: ";
}

void GameRenderer::renderExtendedMenu() const {
    std::cout << "\n=== EXTENDED MENU ===\n";
    std::cout << "1. New Game with Custom Size\n"; 
    std::cout << "2. Switch Combat Type\n";
    std::cout << "3. Buy Spell\n";
    std::cout << "4. Save Game\n";
    std::cout << "5. Return to Game\n";
    std::cout << "Choice: ";
}

void GameRenderer::renderSpellSelectionPrompt(int spellCount) const {
    std::cout << "Select spell by number (1-" << spellCount << "): ";
}

void GameRenderer::renderCoordinatesInputPrompt() const {
    std::cout << "Enter target coordinates (x y): ";
}

void GameRenderer::renderConfirmationPrompt(const std::string& message) const {
    std::cout << message << " (y/n): ";
}

void GameRenderer::renderGameOverMenu() const {
    std::cout << "\nChoose option:\n";
    std::cout << "1. Restart Game\n";
    std::cout << "2. Exit\n";
    std::cout << "Choice: ";
}

void GameRenderer::renderSaveFileInfo(const std::string& saveInfo) const {
    if (saveInfo != "No save file found" && saveInfo != "Corrupted save file") {
        std::cout << "=== UNFINISHED GAME DETECTED ===\n";
        std::cout << saveInfo << "\n";
    } else {
        std::cout << "No previous game found. Starting fresh!\n";
    }
}