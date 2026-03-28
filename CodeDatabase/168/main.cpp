#include <iostream>
#include <conio.h>
#include <windows.h>
#include "Game.h"
#include "GameController.h"
#include "ConsoleView.h"

enum class TargetMode {
    NONE,
    ATTACK,
    SPELL
};

Position handleTargetMode(GameController& game, TargetMode mode, int maxRange) {
    Position playerPos = game.getPlayer().get_Position();
    Position cursor = playerPos;
    const GameField& field = game.getField();

    while (true) {
        ConsoleView::displayTargetMode(game, cursor);
        
        if (mode == TargetMode::ATTACK) {
            std::cout << "  Режим: АТАКА (радиус: " << maxRange << ")\n";
        } else {
            const Spell* spell = game.getPlayerHand().getSelectedSpell();
            if (spell) {
                std::cout << "  Заклинание: " << spell->getName() << " (радиус: " << maxRange << ")\n";
            }
        }

        int input = _getch();
        
        if (input < 0 || input > 127) {
            continue;
        }

        if (input == 0 || input == 224) {
            _getch();
            continue;
        }

        char ch = static_cast<char>(std::toupper(input));
        
        switch (ch) {
        case 'W': {
            Position newPos(cursor.x, cursor.y - 1);
            if (newPos.y >= 0 && 
                std::abs(newPos.x - playerPos.x) <= maxRange && 
                std::abs(newPos.y - playerPos.y) <= maxRange) {
                cursor = newPos;
            }
            break;
        }
        case 'S': {
            Position newPos(cursor.x, cursor.y + 1);
            if (newPos.y < field.getHeight() && 
                std::abs(newPos.x - playerPos.x) <= maxRange && 
                std::abs(newPos.y - playerPos.y) <= maxRange) {
                cursor = newPos;
            }
            break;
        }
        case 'A': {
            Position newPos(cursor.x - 1, cursor.y);
            if (newPos.x >= 0 && 
                std::abs(newPos.x - playerPos.x) <= maxRange && 
                std::abs(newPos.y - playerPos.y) <= maxRange) {
                cursor = newPos;
            }
            break;
        }
        case 'D': {
            Position newPos(cursor.x + 1, cursor.y);
            if (newPos.x < field.getWidth() && 
                std::abs(newPos.x - playerPos.x) <= maxRange && 
                std::abs(newPos.y - playerPos.y) <= maxRange) {
                cursor = newPos;
            }
            break;
        }
        case ' ':
            return cursor;
        case 27:
            return Position(-1, -1);
        }
    }
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    try {
        Game game;
        
        ConsoleView::clearScreen();
        std::cout << "  ===== ДОБРО ПОЖАЛОВАТЬ В ИГРУ! =====\n\n";
        std::cout << "  1. Новая игра\n";
        std::cout << "  2. Загрузить игру\n\n";
        std::cout << "  Выберите действие: ";
        
        char choice = _getch();
        std::cout << choice << "\n\n";
        
        if (choice == '2') {
            if (!game.loadGame()) {
                std::cout << "  Сохранение не найдено или повреждено. Начинаем новую игру.\n";
                _getch();
                game.startNewGame();
            } else {
                std::cout << "  Игра успешно загружена!\n";
                _getch();
            }
        } else {
            game.startNewGame();
        }

        ConsoleView::displayInstructions();
        _getch();

        while (game.getState() != GameState::GAME_OVER && 
               game.getState() != GameState::GAME_WON) {
            
            GameController* level = game.getCurrentLevel();
            if (!level) break;
            
            ConsoleView::displayGameState(*level, game.getCurrentLevelNumber());
            std::cout << "  Введите команду (B - сохранить, ESC - выход): ";
            
            int input = _getch();
            
            if (input < 0 || input > 127) {
                level->setLastMessage("Используйте английскую раскладку!");
                continue;
            }

            if (input == 0 || input == 224) {
                _getch();
                level->setLastMessage("Используйте WASD для движения!");
                continue;
            }

            char ch = static_cast<char>(input);
            
            if (std::toupper(ch) == 'B') {
                try {
                    game.saveGame();
                    _getch();
                } catch (const SaveException& e) {
                    std::cerr << "\n  Ошибка: " << e.what() << "\n";
                    _getch();
                }
                continue;
            }
            
            if (ch == 27) {
                std::cout << "\n  Спасибо за игру!\n";
                break;
            }
            
            std::cout << ch << "\n";

            if (level->getPlayer().get_IsSlowed()) {
                level->getPlayerMutable().remove_Slow();
                level->setLastMessage("Эффект замедления снят! Ход пропущен.");
                level->updateGame();
                game.checkLevelConditions();
                continue;
            }

            bool turnUsed = false;

            switch (std::toupper(ch)) {
            case 'W':
            case 'A':
            case 'S':
            case 'D':
            case 'M':
                game.processPlayerTurn(ch);
                break;
            case 'F': {
                int attackRange = (level->getPlayer().get_CombatMode() == CombatMode::MELEE) ? 1 : 3;
                Position target = handleTargetMode(*level, TargetMode::ATTACK, attackRange);
                if (target.x != -1 && target.y != -1) {
                    if (level->attackAtPosition(target)) {
                        turnUsed = true;
                    }
                } else {
                    level->setLastMessage("Атака отменена");
                }
                break;
            }
            case 'C': {
                if (level->getPlayerHand().getSpellCount() == 0) {
                    level->setLastMessage("Нет доступных заклинаний!");
                    break;
                }
                
                const Spell* spell = level->getPlayerHand().getSelectedSpell();
                if (spell && spell->isEnhancement()) {
                    SpellResult result = level->castSelectedSpell(Position(0, 0));
                    turnUsed = result.consumesTurn;
                } else if (spell) {
                    int spellRange = spell->getRange();
                    EnhancementContext& enh = level->getPlayerHandMutable().getEnhancement();
                    if (enh.hasEnhancement()) {
                        spellRange += enh.bonusRange;
                    }
                    
                    Position target = handleTargetMode(*level, TargetMode::SPELL, spellRange);
                    if (target.x != -1 && target.y != -1) {
                        SpellResult result = level->castSelectedSpell(target);
                        turnUsed = result.consumesTurn;
                    } else {
                        level->setLastMessage("Заклинание отменено");
                    }
                }
                break;
            }
            case 'Q':
                level->selectPreviousSpell();
                level->setLastMessage("Выбрано предыдущее заклинание");
                break;
            case 'E':
                level->selectNextSpell();
                level->setLastMessage("Выбрано следующее заклинание");
                break;
            case '1':
            case '2':
            case '3':
            case '4':
            case '5': {
                int index = ch - '1';
                if (level->selectSpell(index)) {
                    level->setLastMessage("Выбрано заклинание " + std::to_string(index + 1));
                } else {
                    level->setLastMessage("Нет заклинания под этим номером!");
                }
                break;
            }
            default:
                level->setLastMessage("Неизвестная команда! Используйте WASD, F, C, Q, E, M, B");
                continue;
            }

            if (turnUsed) {
                level->updateGame();
                game.checkLevelConditions();
            }
        }

        if (game.getCurrentLevel()) {
            ConsoleView::displayGameOver(*game.getCurrentLevel());
        }
        
        std::cout << "  Нажмите любую клавишу для выхода...\n";
        _getch();

    }
    catch (const GameException& e) {
        std::cerr << "Ошибка игры: " << e.what() << std::endl;
        _getch();
        return 1;
    }
    catch (const std::exception& e) {
        std::cerr << "Неожиданная ошибка: " << e.what() << std::endl;
        _getch();
        return 1;
    }

    return 0;
}
