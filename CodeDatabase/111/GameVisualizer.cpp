#include "GameVisualizer.h"
#include <iostream>

void ConsoleVisualization::displayGameState(const GameBoard& board, 
                                           const Hero& hero, 
                                           const std::vector<Foe>& enemies,
                                           const std::vector<EnemyTower>& towers) {
    std::cout << "\n--- GAME BOARD ---\n";
    displayBoard(board, hero, enemies, towers);
    displayStats(hero);
    if (!enemies.empty()) displayEnemiesInfo(enemies);
    if (!towers.empty()) displayTowersInfo(towers);
    displaySpells(hero); 
    std::cout << "\n";
}

void ConsoleVisualization::showMessage(const std::string& message) {
    std::cout << message << "\n";
}

void ConsoleVisualization::showMenu() {
    std::cout << "\n=== MAIN MENU ===\n";
    std::cout << "1. New Game\n";
    std::cout << "2. Load Game\n";
    std::cout << "3. Exit\n";
    std::cout << "Choose option: ";
}

void ConsoleVisualization::showHelp() {
    std::cout << "\n=== GAME HELP ===\n";
    std::cout << "Legend: H-Hero, E-Enemy, T-Tower, #-Wall, *-Trap, .-Empty\n";
    std::cout << "Combat: Melee (close) / Ranged (distance)\n";
    std::cout << "Spells: Can be purchased in shop with mana\n";
    std::cout << "Spell commands: m1, m2, m3... followed by target coordinates\n";
}

void ConsoleVisualization::showPlayerTurn(const Hero& hero) {
    std::cout << "\n=== YOUR TURN ===\n";
    std::cout << "Health: " << hero.getHitpoints() << "/" << hero.getMaxHitpoints() << "\n";
    std::cout << "Combat: " << (hero.isMeleeMode() ? "Melee" : "Ranged") << "\n";
    std::cout << "Mana: " << hero.getSpellHand().getMana() << "\n";
    std::cout << "Enemies defeated: " << hero.getEnemiesDefeated() << "\n";
}

void ConsoleVisualization::showVictoryScreen() {
    std::cout << "\n"
        " ╔══════════════════════════════╗\n"
        " ║          VICTORY!           ║\n"
        " ║   All enemies defeated!     ║\n"
        " ║    Towers destroyed!        ║\n"
        " ╚══════════════════════════════╝\n";
}

void ConsoleVisualization::showDefeatScreen() {
    std::cout << "\n"
        " ╔══════════════════════════════╗\n"
        " ║          DEFEAT!            ║\n"
        " ║     Hero has fallen!        ║\n"
        " ╚══════════════════════════════╝\n";
}

void ConsoleVisualization::displayBoard(const GameBoard& board, 
                                       const Hero& hero, 
                                       const std::vector<Foe>& enemies,
                                       const std::vector<EnemyTower>& towers) const {
    for (size_t row = 0; row < board.getWidth(); ++row) {
        for (size_t col = 0; col < board.getHeight(); ++col) {
            Location loc(row, col);
            
            if (!board.isLocationValid(loc)) {
                std::cout << " ";
            } else if (board.isObstacle(loc)) {
                std::cout << "#";
            } else if (loc == hero.getPosition()) {
                std::cout << "H";
            } else {
                bool actorFound = false;
                
                for (const auto& foe : enemies) {
                    if (foe.isAlive() && foe.getPosition() == loc) {
                        std::cout << "E";
                        actorFound = true;
                        break;
                    }
                }
                
                if (!actorFound) {
                    for (const auto& tower : towers) {
                        if (tower.isAlive() && tower.getPosition() == loc) {
                            std::cout << "T";
                            actorFound = true;
                            break;
                        }
                    }
                }
                
                if (!actorFound) {
                    GameActor* actor = board.getActorAt(loc);
                    if (actor && actor->isAlive()) {
                        std::cout << "*";
                        actorFound = true;
                    }
                }
                
                if (!actorFound) {
                    std::cout << ".";
                }
            }
            std::cout << " ";
        }
        std::cout << "\n";
    }
}

void ConsoleVisualization::displayStats(const Hero& hero) const {
    std::cout << "\nPLAYER:\n";
    std::cout << "  Position: (" << hero.getPosition().row << "," << hero.getPosition().col << ")\n";
    std::cout << "  HP: " << hero.getHitpoints() << "/" << hero.getMaxHitpoints() << "\n";
    std::cout << "  Mode: " << (hero.isMeleeMode() ? "Melee" : "Ranged") << "\n";
    std::cout << "  Damage: " << hero.getCurrentDamage() << "\n";
}

void ConsoleVisualization::displayEnemiesInfo(const std::vector<Foe>& enemies) const {
    std::cout << "\nENEMIES:\n";
    int aliveCount = 0;
    for (size_t i = 0; i < enemies.size(); ++i) {
        const Foe& foe = enemies[i];
        if (foe.isAlive()) {
            aliveCount++;
            std::cout << "  Enemy " << (i + 1) << " at (" << foe.getPosition().row 
                      << "," << foe.getPosition().col << ") HP: "
                      << foe.getHitpoints() << "/" << foe.getMaxHitpoints() 
                      << " DMG: " << foe.getAttackPower() << "\n";
        }
    }
    if (aliveCount == 0) {
        std::cout << "  No alive enemies\n";
    }
}

void ConsoleVisualization::displayTowersInfo(const std::vector<EnemyTower>& towers) const {
    std::cout << "\nTOWERS:\n";
    int aliveCount = 0;
    for (size_t i = 0; i < towers.size(); ++i) {
        const EnemyTower& tower = towers[i];
        if (tower.isAlive()) {
            aliveCount++;
            std::cout << "  Tower " << (i + 1) << " at (" << tower.getPosition().row 
                      << "," << tower.getPosition().col << ") HP: "
                      << tower.getHitpoints() << "/" << tower.getMaxHitpoints() 
                      << " Range: " << tower.getAttackRange() 
                      << " DMG: " << tower.getAttackPower() << "\n";
        }
    }
    if (aliveCount == 0) {
        std::cout << "  No alive towers\n";
    }
}

void ConsoleVisualization::displaySpells(const Hero& hero) {
    const SpellHand& spellHand = hero.getSpellHand();
    
    std::cout << "\nSPELLS (" << spellHand.getSpellCount() 
              << "/" << spellHand.getMaxSize() << "):\n";
    
    if (spellHand.getSpellCount() == 0) {
        std::cout << "  No spells - type 'shop' to buy some!\n";
    } else {
        for (size_t i = 0; i < spellHand.getSpellCount(); ++i) {
            const Spell* spell = spellHand.getSpell(i);
            if (spell) {
                std::cout << "  m" << (i + 1) << ". " << spell->getName() 
                          << " - " << spell->getDescription() 
                          << " [Range: " << spell->getRange() 
                          << ", Cost: " << spell->getManaCost() << "]\n";
            }
        }
    }
}