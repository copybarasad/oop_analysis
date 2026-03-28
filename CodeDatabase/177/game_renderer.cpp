#include "game_renderer.h"
#include "spell_card.h"
#include "direct_damage_spell.h"
#include "area_damage_spell.h"
#include "trap_spell.h"
#include <iomanip>
#include <algorithm>

GameRenderer::GameRenderer(std::ostream& stream)
    : outputStream(stream) {}

void GameRenderer::renderSeparator(int width) const {
    outputStream << std::string(width, '=') << std::endl;
}

void GameRenderer::renderPlayerInfo(const Player& player) const {
    outputStream << "PLAYER:" << std::endl;
    outputStream << "  Health: " << player.getHealth() << " HP" << std::endl;
    outputStream << "  Mana: " << player.getMana() << "/" << player.getMaxMana() << " MP" << std::endl;
    outputStream << "  Mode: " << (player.getCombatMode() == CombatMode::MELEE ? "⚔️ MELEE" : "🏹 RANGED") << std::endl;
    outputStream << "  Score: " << player.getScore() << " points" << std::endl;
    outputStream << "  Position: (" << player.getPosition().getX() << ", " << player.getPosition().getY() << ")" << std::endl;
}

void GameRenderer::renderSpells(const Player& player) const {
    outputStream << "\nSPELLS:" << std::endl;
    if (player.getSpellHand().getSpellCount() > 0) {
        for (int i = 0; i < player.getSpellHand().getSpellCount(); ++i) {
            SpellCard* spell = player.getSpellHand().getSpell(i);
            if (spell) {
                outputStream << "  " << i << ". " << spell->getName() << " (Cost: " << spell->getManaCost() << " MP)";
                if (auto* directSpell = dynamic_cast<DirectDamageSpell*>(spell)) {
                    outputStream << " - Damage: " << directSpell->getDamage();
                } else if (auto* areaSpell = dynamic_cast<AreaDamageSpell*>(spell)) {
                    outputStream << " - Area: " << areaSpell->getAreaSize() << "x" << areaSpell->getAreaSize();
                } else if (dynamic_cast<TrapSpell*>(spell)) {
                    outputStream << " - Trap";
                }
                outputStream << std::endl;
            }
        }
    } else {
        outputStream << "  No spells available" << std::endl;
    }
}

void GameRenderer::renderEnemiesInfo(const GameController& game) const {
    outputStream << "\nENEMIES & OBJECTS:" << std::endl;
    outputStream << "  Enemies: " << game.getEnemies().size() << std::endl;
    
    // Показываем информацию о каждом враге
    const auto& enemies = game.getEnemies();
    if (!enemies.empty()) {
        outputStream << "    Enemy positions:" << std::endl;
        for (size_t i = 0; i < enemies.size() && i < 5; ++i) {
            const auto& enemy = enemies[i];
            outputStream << "      [" << i << "] HP: " << enemy.getHealth() 
                        << ", DMG: " << enemy.getDamage()
                        << ", Pos: (" << enemy.getPosition().getX() 
                        << ", " << enemy.getPosition().getY() << ")" << std::endl;
        }
        if (enemies.size() > 5) {
            outputStream << "      ... and " << (enemies.size() - 5) << " more" << std::endl;
        }
    }
    
    outputStream << "  Buildings: " << game.getBuildings().size() << std::endl;
    outputStream << "  Towers: " << game.getTowers().size() << std::endl;
    
    // Активные ловушки
    const auto& traps = game.getActiveTraps();
    if (!traps.empty()) {
        outputStream << "  Active Traps: " << traps.size() << std::endl;
    }
}

void GameRenderer::renderCommands() const {
    outputStream << "\nCOMMANDS:" << std::endl;
    outputStream << "  Movement: W,A,S,D" << std::endl;
    outputStream << "  Switch Mode: M" << std::endl;
    outputStream << "  Cast Spell: C" << std::endl;
    outputStream << "  Buy Spell: B" << std::endl;
    outputStream << "  Game Menu: P" << std::endl;
    outputStream << "  Quit: Q" << std::endl;
}

char GameRenderer::getCellSymbol(const GameController& game, int x, int y) const {
    Position pos(x, y);
    const GameField& field = game.getField();
    
    // Проверяем игрока
    if (game.getPlayer().getPosition().getX() == x && 
        game.getPlayer().getPosition().getY() == y) {
        return 'P'; 
    }
    
    // Проверяем врагов
    for (const auto& enemy : game.getEnemies()) {
        if (enemy.getPosition().getX() == x && enemy.getPosition().getY() == y) {
            return 'E'; 
        }
    }
    
    // Проверяем здания
    for (const auto& building : game.getBuildings()) {
        if (building.getPosition().getX() == x && building.getPosition().getY() == y) {
            return 'B'; 
        }
    }
    
    // Проверяем башни
    for (const auto& tower : game.getTowers()) {
        if (tower.getPosition().getX() == x && tower.getPosition().getY() == y) {
            return 'T'; 
        }
    }
    
    // Проверяем ловушки
    for (const auto& trapPos : game.getActiveTraps()) {
        if (trapPos.getX() == x && trapPos.getY() == y) {
            return 'X'; 
        }
    }
    
    // Проверяем тип клетки
    if (!field.isPositionPassable(pos)) {
        return '#'; 
    }
    
    const GameCell& cell = field.getCell(pos);
    switch (cell.getType()) {
        case CellType::OBSTACLE:
            return '#';
        case CellType::SLOW:
            return '~';
        case CellType::EMPTY:
        default:
            return '.';
    }
}

void GameRenderer::renderFieldHeader(int width) const {
    outputStream << "  ";
    for (int i = 0; i < width; ++i) {
        outputStream << std::setw(2) << i;
    }
    outputStream << std::endl;
}

void GameRenderer::renderFieldFooter(int width) const {
    outputStream << "  ";
    for (int i = 0; i < width; ++i) {
        outputStream << std::setw(2) << i;
    }
    outputStream << std::endl;
}

void GameRenderer::renderField(const GameController& game) const {
    const GameField& field = game.getField();
    int width = field.getWidth();
    int height = field.getHeight();
    
    outputStream << "\nGAME FIELD:" << std::endl;
    renderFieldHeader(width);
    
    for (int y = 0; y < height; ++y) {
        outputStream << std::setw(2) << y << " ";
        for (int x = 0; x < width; ++x) {
            outputStream << std::setw(2) << getCellSymbol(game, x, y);
        }
        outputStream << std::endl;
    }
    
    renderFieldFooter(width);
    
    outputStream << "\nLEGEND:" << std::endl;
    outputStream << "  P = Player, E = Enemy, B = Building, T = Tower, X = Trap" << std::endl;
    outputStream << "  # = Obstacle, ~ = Slow, . = Empty" << std::endl;
}

void GameRenderer::renderGameState(const GameController& game) const {
    const Player& player = game.getPlayer();
    
    outputStream << "\n";
    renderSeparator(50);
    outputStream << "=== LEVEL " << game.getCurrentLevel() << " ===" << std::endl;
    renderSeparator(50);
    
    renderPlayerInfo(player);
    renderSpells(player);
    renderEnemiesInfo(game);
    renderField(game);
    renderCommands();
    
    renderSeparator(50);
}

void GameRenderer::renderMainMenu() const {
    outputStream << "\n=== MAIN MENU ===" << std::endl;
    outputStream << "1. New Game" << std::endl;
    outputStream << "2. Load Game" << std::endl;
    outputStream << "3. Exit" << std::endl;
}

void GameRenderer::renderLoadMenu(const std::vector<std::string>& saveFiles) const {
    outputStream << "\n=== LOAD GAME ===" << std::endl;
    if (saveFiles.empty()) {
        outputStream << "No save files found." << std::endl;
        return;
    }
    
    for (size_t i = 0; i < saveFiles.size(); ++i) {
        outputStream << (i + 1) << ". " << saveFiles[i] << std::endl;
    }
    outputStream << "0. Back to Main Menu" << std::endl;
}

void GameRenderer::renderInGameMenu() const {
    outputStream << "\n=== GAME MENU ===" << std::endl;
    outputStream << "S. Save Game" << std::endl;
    outputStream << "M. Main Menu" << std::endl;
    outputStream << "C. Continue" << std::endl;
}

void GameRenderer::renderGameOverMenu(const GameController& game) const {
    if (game.getPlayer().isAlive()) {
        outputStream << "\n=== LEVEL COMPLETED ===" << std::endl;
        outputStream << "Score: " << game.getPlayer().getScore() << std::endl;
    } else {
        outputStream << "\n=== GAME OVER ===" << std::endl;
        outputStream << "Final Score: " << game.getPlayer().getScore() << std::endl;
    }
    
    outputStream << "1. New Game" << std::endl;
    outputStream << "2. Main Menu" << std::endl;
    outputStream << "3. Exit" << std::endl;
}

void GameRenderer::renderLevelStart(int level, const LevelConfig& config) const {
    outputStream << "\n";
    renderSeparator(50);
    outputStream << "🎯 STARTING LEVEL " << level << std::endl;
    renderSeparator(50);
    outputStream << "📐 Field Size: " << config.fieldWidth << "x" << config.fieldHeight << std::endl;
    outputStream << "👹 Enemies: " << config.enemyCount << " (HP: " << config.enemyHealth 
                 << ", DMG: " << config.enemyDamage << ")" << std::endl;
    outputStream << "🏠 Buildings: " << config.buildingCount << std::endl;
    renderSeparator(50);
}

void GameRenderer::renderLevelComplete(int level, int score) const {
    outputStream << "\n";
    renderSeparator(50);
    outputStream << "🎉 LEVEL " << level << " COMPLETED! 🎉" << std::endl;
    outputStream << "Score: " << score << " points" << std::endl;
    renderSeparator(50);
}

void GameRenderer::renderVictory(int finalScore) const {
    outputStream << "\n";
    renderSeparator(50);
    outputStream << "=== ПОБЕДА! ВСЕ УРОВНИ ПРОЙДЕНЫ! ===" << std::endl;
    outputStream << "Финальный счет: " << finalScore << " очков!" << std::endl;
    renderSeparator(50);
}

void GameRenderer::renderMessage(const std::string& message) const {
    outputStream << message << std::endl;
}

void GameRenderer::renderError(const std::string& error) const {
    outputStream << "\n*** ERROR: " << error << " ***" << std::endl;
}

void GameRenderer::renderPlayerStats(const Player& player) const {
    renderPlayerInfo(player);
}

void GameRenderer::renderFieldGrid(const GameController& game) const {
    renderField(game);
}
