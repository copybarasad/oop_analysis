#include "ConsoleRenderer.hpp"
#include "Configs.hpp"
#include <iostream>

// Отрисовка игрового поля
void ConsoleRenderer::render(const Field& field) {
    std::cout << "=== GAME FIELD ===" << std::endl;

    for (int y = 0; y < field.getHeight(); y++) {
        for (int x = 0; x < field.getWidth(); x++) {
            Pos currentPos(x, y);
            char symbol = '.';

            // Определение символа для текущей позиции
            if (currentPos == field.getPlayerPosition()) {
                symbol = Config::Graphics::PLAYER_SYMBOL;
            } else if (field.isEnemyAt(currentPos)) {
                symbol = Config::Graphics::ENEMY_SYMBOL;
            } else {
                CellType cellType = field.getCellType(currentPos);
                symbol = getCellSymbol(cellType);
            }

            std::cout << symbol << ' ';
        }
        std::cout << std::endl;
    }
}

// Отрисовка статистики игрока
void ConsoleRenderer::renderStats(const Player& player) {
    std::cout << "HP: " << player.getHealth()
              << " | Damage: " << player.getDamage()
              << " | Score: " << player.getScore()
              << " | Combat Mode: " << (player.getIsRangedMode() ? "Ranged" : "Melee") << std::endl;
    std::cout << "Commands: w/a/s/d - Move, i/j/k/l - Attack, c - Switch Combat Mode, u/y - Cast Spell, save - Save, m - Pause Menu, q - Quit" << std::endl;

    // Отображение заклинаний в руке
    const Hand& hand = player.getHand();
    std::cout << "Hand (" << hand.size() << "/" << hand.capacity() << "): ";
    if (hand.size() == 0) {
        std::cout << "(empty)";
    } else {
        for (size_t i = 0; i < hand.size(); ++i) {
            const ISpell* s = hand.getSpell(i);
            if (s) {
                std::cout << "[" << i << ":" << s->name() << "] ";
            } else {
                std::cout << "[" << i << ":<empty>] ";
            }
        }
    }
    std::cout << std::endl;
}

// Отображение главного меню
void ConsoleRenderer::renderMainMenu() {
    std::cout << "=== MAIN MENU ===" << std::endl;
    std::cout << "1. New Game" << std::endl;
    std::cout << "2. Load Game" << std::endl;
    std::cout << "3. Quit" << std::endl;
    std::cout << "Select option (1-3): ";
}

// Отображение меню паузы
void ConsoleRenderer::renderPauseMenu() {
    std::cout << "=== PAUSE MENU ===" << std::endl;
    std::cout << "1. Continue Game" << std::endl;
    std::cout << "2. Save Game" << std::endl;
    std::cout << "3. Main Menu" << std::endl;
    std::cout << "4. Quit" << std::endl;
    std::cout << "Select option (1-4): ";
}

// Отображение меню загрузки
void ConsoleRenderer::renderLoadMenu(const std::vector<SaveInfo>& saves) {
    std::cout << "=== LOAD GAME ===" << std::endl;
    if (saves.empty()) {
        std::cout << "No save files found." << std::endl;
        std::cout << "Press 'm' to return to main menu: ";
    } else {
        for (size_t i = 0; i < saves.size(); ++i) {
            std::cout << i + 1 << ". " << saves[i].filename 
                      << " (Level " << saves[i].level 
                      << ", Score: " << saves[i].playerScore 
                      << ", " << saves[i].timestamp << ")" << std::endl;
        }
        std::cout << "Select save file (1-" << saves.size() << ") or 'm' to return to start menu: ";
    }
}

// Отображение экрана завершения игры
void ConsoleRenderer::renderGameOver(bool victory, int score) {
    if (victory) {
        std::cout << "=== VICTORY ===" << std::endl;
        std::cout << "Congratulations! You won!" << std::endl;
    } else {
        std::cout << "=== GAME OVER ===" << std::endl;
        std::cout << "You have been defeated." << std::endl;
    }
    std::cout << "Final score: " << score << std::endl;
}

// Получение символа для типа клетки
char ConsoleRenderer::getCellSymbol(CellType type) const {
    switch (type) {
        case CellType::EMPTY: return Config::Graphics::EMPTY_CELL_SYMBOL;
        case CellType::OBSTACLE: return Config::Graphics::OBSTACLE_SYMBOL;
        default: return '?';
    }
}
