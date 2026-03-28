#include "ConsoleView.h"
#include <iostream>
#include <iomanip>
#include <windows.h>

void ConsoleView::clearScreen() {
    system("cls");
}

void ConsoleView::displayGameState(const GameController& game, int currentLevel) {
    clearScreen();
    
    const Player& player = game.getPlayer();
    const GameField& field = game.getField();
    const PlayerHand& hand = game.getPlayerHand();

    std::cout << "\n";
    std::cout << "  ╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "  ║                          THE GAME                          ║\n";
    std::cout << "  ╚════════════════════════════════════════════════════════════╝\n\n";

    std::cout << "  ┌─────────────────── ИГРОК ───────────────────┐\n";
    std::cout << "  │    Уровень: " << std::setw(2) << currentLevel;
    std::cout << "  │    HP: " << std::setw(3) << player.get_Health() << "/" << std::setw(3) << player.get_MaxHealth();
    std::cout << "  │    Очки: " << std::setw(4) << player.get_Score() << " │\n";
    std::cout << "  │    Урон: " << std::setw(2) << player.get_Damage();
    std::cout << "  │    Режим: " << (player.get_CombatMode() == CombatMode::MELEE ? "Ближний " : "Дальний");
    std::cout << "  │ " << (player.get_IsSlowed() ? " ЗАМЕДЛЕН" : "          ") << "       │\n";
    std::cout << "  └──────────────────────────────────────────────┘\n\n";


    std::cout << "  ┌─────────────────── УРОВЕНЬ " << std::setw(2) << currentLevel << " ───────────────────┐\n";
    std::cout << "  │   Врагов: " << std::setw(2) << game.getEnemies().size();
    std::cout << "  │   Зданий: " << std::setw(2) << game.getBuildings().size();
    std::cout << "  │   Башен: " << std::setw(2) << game.getTowers().size() << " │\n";
    std::cout << "  │   Союзников: " << std::setw(2) << game.getAllies().size();
    std::cout << "  │   Ловушек: " << std::setw(2) << game.getTraps().size() << "           │\n";
    std::cout << "  │   Цель: Уничтожить всех врагов, здания и башни │\n";
    std::cout << "  └──────────────────────────────────────────────┘\n\n";

    std::cout << "  ";
    for (int x = 0; x < field.getWidth(); ++x) {
        std::cout << "──";
    }
    std::cout << "─\n";

    for (int y = 0; y < field.getHeight(); ++y) {
        std::cout << "  │";
        for (int x = 0; x < field.getWidth(); ++x) {
            char symbol = '.';
            
            if (x == player.get_Position().x && y == player.get_Position().y) {
                std::cout << "P ";
                continue;
            }

            const Cell& cell = field.getCell(x, y);

            if (cell.has_Ally()) {
                std::cout << "A ";
            }
            else if (cell.has_Enemy()) {
                std::cout << "E ";
            }
            else if (cell.has_Building()) {
                std::cout << "B ";
            }
            else if (cell.has_Tower()) {
                std::cout << "T ";
            }
            else if (cell.has_Trap()) {
                std::cout << "^ ";
            }
            else {
                switch (cell.get_type()) {
                case CellType::OBSTACLE: 
                    std::cout << "# "; 
                    break;
                case CellType::SLOWERING: 
                    std::cout << "~ "; 
                    break;
                default: 
                    std::cout << ". "; 
                    break;
                }
            }
        }
        std::cout << "│\n";
    }

    std::cout << "  ";
    for (int x = 0; x < field.getWidth(); ++x) {
        std::cout << "──";
    }
    std::cout << "─\n\n";

    std::cout << "  ┌─────────────────── ЛЕГЕНДА  ─────────────────┐\n";
    std::cout << "  │ P-Игрок  E-Враг   B-Здание  T-Башня          │\n";
    std::cout << "  │ A-Союзник ^-Ловушка #-Стена  ~-Замедление    │\n";
    std::cout << "  └──────────────────────────────────────────────┘\n\n";

    displaySpellInfo(game);

    if (!game.getLastMessage().empty()) {
        std::cout << "\n  " << game.getLastMessage() << "\n";
    }
}

void ConsoleView::displaySpellInfo(const GameController& game) {
    const PlayerHand& hand = game.getPlayerHand();

    std::cout << "  ┌─────────────────── ЗАКЛИНАНИЯ ──────────────┐\n";
    
    if (hand.getSpellCount() == 0) {
        std::cout << "  │ Нет доступных заклинаний                    │\n";
    } else {
        for (int i = 0; i < hand.getSpellCount(); ++i) {
            const Spell* spell = hand.getSpell(i);
            if (spell) {
                std::string marker = (i == hand.getSelectedIndex()) ? "►" : " ";
                std::cout << "  │ " << marker << " [" << (i + 1) << "] " 
                          << spell->getSymbol() << " " 
                          << std::setw(25) << std::left << spell->getName();
                
                if (spell->getRange() > 0) {
                    std::cout << " R:" << spell->getRange();
                }
                std::cout << std::setw(5) << " " << "│\n";
            }
        }
    }
    
    std::cout << "  │ Места: " << hand.getSpellCount() << "/" << hand.getMaxSize();
    std::cout << "  │ До нового: " << hand.getKillsUntilNextSpell() << " убийств";

    const EnhancementContext& enh = const_cast<PlayerHand&>(game.getPlayerHand()).getEnhancement();
    if (enh.hasEnhancement()) {
        std::cout << " │ x" << enh.stackCount;
    }
    std::cout << "  │\n";
    
    std::cout << "  └──────────────────────────────────────────────┘\n";
}

void ConsoleView::displayTargetMode(const GameController& game, const Position& cursor) {
    clearScreen();
    
    const Player& player = game.getPlayer();
    const GameField& field = game.getField();

    std::cout << "\n  РЕЖИМ ПРИЦЕЛИВАНИЯ \n";
    std::cout << "  Используйте WASD для выбора цели, SPACE для подтверждения, ESC для отмены\n\n";

    std::cout << "  ";
    for (int x = 0; x < field.getWidth(); ++x) {
        std::cout << "──";
    }
    std::cout << "─\n";

    for (int y = 0; y < field.getHeight(); ++y) {
        std::cout << "  │";
        for (int x = 0; x < field.getWidth(); ++x) {
            if (x == cursor.x && y == cursor.y) {
                std::cout << "X ";
                continue;
            }
            
            if (x == player.get_Position().x && y == player.get_Position().y) {
                std::cout << "P ";
                continue;
            }

            const Cell& cell = field.getCell(x, y);

            if (cell.has_Ally()) {
                std::cout << "A ";
            }
            else if (cell.has_Enemy()) {
                std::cout << "E ";
            }
            else if (cell.has_Building()) {
                std::cout << "B ";
            }
            else if (cell.has_Tower()) {
                std::cout << "T ";
            }
            else if (cell.has_Trap()) {
                std::cout << "^ ";
            }
            else {
                switch (cell.get_type()) {
                case CellType::OBSTACLE: 
                    std::cout << "# "; 
                    break;
                case CellType::SLOWERING: 
                    std::cout << "~ "; 
                    break;
                default: 
                    std::cout << ". "; 
                    break;
                }
            }
        }
        std::cout << "│\n";
    }

    std::cout << "  ";
    for (int x = 0; x < field.getWidth(); ++x) {
        std::cout << "──";
    }
    std::cout << "─\n";
    
    std::cout << "\n  Позиция курсора: (" << cursor.x << ", " << cursor.y << ")\n";
}

void ConsoleView::displayGameOver(const GameController& game) {
    clearScreen();
    
    std::cout << "\n\n";
    std::cout << "  ╔════════════════════════════════════════════════════════════╗\n";
    
    if (game.isGameWon()) {
        std::cout << "  ║                     ПОБЕДА!                            ║\n";
        std::cout << "  ║              Все враги уничтожены!                         ║\n";
    } else {
        std::cout << "  ║                     ПОРАЖЕНИЕ                          ║\n";
        std::cout << "  ║                   Вы были побеждены                        ║\n";
    }
    
    std::cout << "  ╠════════════════════════════════════════════════════════════╣\n";
    std::cout << "  ║        Финальный счёт: " << std::setw(6) << game.getPlayer().get_Score() 
              << "                           ║\n";
    std::cout << "  ╚════════════════════════════════════════════════════════════╝\n\n";
}

void ConsoleView::displayInstructions() {
    std::cout << "\n";
    std::cout << "  ╔════════════════════════════════════════════════════════════╗\n";
    std::cout << "  ║                    УПРАВЛЕНИЕ                              ║\n";
    std::cout << "  ╠════════════════════════════════════════════════════════════╣\n";
    std::cout << "  ║  WASD      - Передвижение                                  ║\n";
    std::cout << "  ║  F         - Атаковать (ближний: 1, дальний: 3 клетки)      ║\n";
    std::cout << "  ║  C         - Использовать выбранное заклинание             ║\n";
    std::cout << "  ║  1-5       - Выбрать заклинание по номеру                  ║\n";
    std::cout << "  ║  Q/E       - Предыдущее/следующее заклинание               ║\n";
    std::cout << "  ║  M         - Сменить режим боя (ближний/дальний)           ║\n";
    std::cout << "  ║  B         - Сохранить игру                                ║\n";
    std::cout << "  ║  ESC       - Выход из игры                                 ║\n";
    std::cout << "  ╠════════════════════════════════════════════════════════════╣\n";
    std::cout << "  ║  В режиме прицеливания:                                    ║\n";
    std::cout << "  ║  WASD      - Двигать прицел (в пределах радиуса)           ║\n";
    std::cout << "  ║  SPACE     - Подтвердить                                   ║\n";
    std::cout << "  ║  ESC       - Отмена                                        ║\n";
    std::cout << "  ╠════════════════════════════════════════════════════════════╣\n";
    std::cout << "  ║  Заклинания: D-Урон, A-Площадь, T-Ловушка, U-Призыв, E-Усил║\n";
    std::cout << "  ╚════════════════════════════════════════════════════════════╝\n\n";
    std::cout << "  ВАЖНО: Используйте АНГЛИЙСКУЮ раскладку клавиатуры!\n\n";
    std::cout << "  Нажмите любую клавишу для начала игры...\n";
}
