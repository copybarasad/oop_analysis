#include "console_renderer.h"
#include "upgrade_system.h"
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <optional>
#include <vector>
#include <iomanip>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

namespace rpg {
    void ConsoleRenderer::ClearScreen() const {
#ifdef _WIN32
        system("cls");
#else
        std::cout << "\033[2J\033[1;1H";
#endif
    }

    void ConsoleRenderer::RenderMessage(const std::string &message) const {
        std::cout << "➤ " << message << "\n\n";
    }

    void ConsoleRenderer::RenderMainMenu() const {
        std::cout << "\n=== RPG ИГРА ===\n";
        std::cout << "1. Новая игра\n";
        std::cout << "2. Загрузить игру\n";
        std::cout << "3. Выход\n";
        std::cout << "Выберите опцию: ";
    }

    void ConsoleRenderer::RenderUpgradeMenu(const std::vector<Upgrade> &upgrades) const {
        std::cout << "\n=== ПРОКАЧКА ПЕРСОНАЖА ===\n";
        std::cout << "Выберите улучшение:\n";
        for (size_t i = 0; i < upgrades.size(); ++i) {
            std::cout << (i + 1) << ". " << upgrades[i].name << " - " 
                      << upgrades[i].description << "\n";
        }
        std::cout << "Ваш выбор: ";
    }

    void ConsoleRenderer::RenderLevelComplete(int score) const {
        std::cout << "\n╔═══════════════════════════════════════════════════════════════════╗\n";
        std::cout << "║                        УРОВЕНЬ ПРОЙДЕН!                           ║\n";
        std::cout << "╠═══════════════════════════════════════════════════════════════════╣\n";
        std::cout << "║  Очков набрано: " << std::setw(50) << std::left << score << "║\n";
        std::cout << "╚═══════════════════════════════════════════════════════════════════╝\n";
    }

    void ConsoleRenderer::RenderGameComplete(int final_score) const {
        std::cout << "\n╔═══════════════════════════════════════════════════════════════════╗\n";
        std::cout << "║                        ПОЗДРАВЛЯЕМ!                                 ║\n";
        std::cout << "╠═══════════════════════════════════════════════════════════════════╣\n";
        std::cout << "║  Вы прошли все уровни!                                             ║\n";
        std::cout << "║  Финальный счёт: " << std::setw(48) << std::left << final_score << "║\n";
        std::cout << "╚═══════════════════════════════════════════════════════════════════╝\n";
    }

    void ConsoleRenderer::RenderLevelStart(int level_number) const {
        std::cout << "\n╔═══════════════════════════════════════════════════════════════════╗\n";
        std::cout << "║                    УРОВЕНЬ " << std::setw(2) << std::right << level_number 
                  << " НАЧАТ!                              ║\n";
        std::cout << "╚═══════════════════════════════════════════════════════════════════╝\n";
    }

    void ConsoleRenderer::RenderGameOver(int score, int turns, int level) const {
        std::cout << "\n╔═══════════════════════════════════════════════════════════════════╗\n";
        std::cout << "║                        ИГРА ОКОНЧЕНА                              ║\n";
        std::cout << "╠═══════════════════════════════════════════════════════════════════╣\n";
        std::cout << "║  Вы погибли!                                                      ║\n";
        std::cout << "║  Финальные очки: " << std::setw(49) << std::left << score << "║\n";
        std::cout << "║  Пройдено ходов: " << std::setw(49) << std::left << turns << "║\n";
        std::cout << "║  Достигнутый уровень: " << std::setw(44) << std::left << level << "║\n";
        std::cout << "╚═══════════════════════════════════════════════════════════════════╝\n";
    }

    void ConsoleRenderer::RenderHeader() const {
        std::cout << "╔═══════════════════════════════════════════════════════════════════╗\n";
        std::cout << "║                    RPG ИГРА - КОНСОЛЬНАЯ ВЕРСИЯ                   ║\n";
        std::cout << "╠═══════════════════════════════════════════════════════════════════╣\n";
        std::cout << "║  Управление: w/a/s/d - движение, e - атака по области             ║\n";
        std::cout << "║              c - сменить режим боя, 0-4 - использовать заклинание ║\n";
        std::cout << "║              b - купить заклинание, v - сохранить, q - выход      ║\n";
        std::cout << "╚═══════════════════════════════════════════════════════════════════╝\n\n";
    }

    void ConsoleRenderer::RenderGameState(const Game &game) const {
        const Player &player = game.GetPlayer();
        Position player_pos = player.GetPosition();

        std::cout << "=== Уровень " << game.GetCurrentLevel() << " ===\n";
        std::cout << "Игрок: HP=" << player.GetHealth() << "/" << player.GetMaxHealth()
                << ", Урон=" << player.GetDamage()
                << ", Режим=" << (player.GetCombatMode() == CombatMode::kMelee ? "Ближний" : "Дальний")
                << ", Позиция=(" << player_pos.x << "," << player_pos.y << "), Очки=" << player.GetScore() << "\n";
        std::cout << "Врагов: " << game.GetEnemies().size()
                << ", Союзников: " << game.GetAllies().size()
                << ", Ловушек: " << game.GetTraps().size()
                << ", Зданий: " << game.GetBuildings().size()
                << ", Башен: " << game.GetTowers().size() << "\n";

        const auto &spells = player.GetSpellHand();
        std::cout << "Заклинания (" << spells.GetSize() << "/" << spells.GetMaxSize() << "): ";
        for (size_t i = 0; i < spells.GetSize(); ++i) {
            const Spell *spell = spells.GetSpell(i);
            std::cout << "[" << i << "] " << spell->GetName() << " ";
        }
        std::cout << "\n";
    }

    void ConsoleRenderer::RenderMap(const Game &game) const {
        const GameField &field = game.GetField();
        const Player &player = game.GetPlayer();
        const auto &enemies = game.GetEnemies();
        const auto &buildings = game.GetBuildings();
        const auto &traps = game.GetTraps();
        const auto &allies = game.GetAllies();
        const auto &towers = game.GetTowers();

        std::cout << "\n=== Карта ===\n";

        Position player_pos = player.GetPosition();

        for (size_t y = 0; y < field.GetHeight(); ++y) {
            for (size_t x = 0; x < field.GetWidth(); ++x) {
                Position current_pos(x, y);

                if (player_pos == current_pos) {
                    std::cout << GetCharFor(player) << " ";
                    continue;
                }

                bool enemy_found = false;
                for (const auto &enemy: enemies) {
                    if (enemy.GetPosition() == current_pos && enemy.IsAlive()) {
                        std::cout << GetCharFor(enemy) << " ";
                        enemy_found = true;
                        break;
                    }
                }
                if (enemy_found) continue;

                bool ally_found = false;
                for (const auto &ally: allies) {
                    if (ally.GetPosition() == current_pos && ally.IsAlive()) {
                        std::cout << GetCharFor(ally) << " ";
                        ally_found = true;
                        break;
                    }
                }
                if (ally_found) continue;

                bool building_found = false;
                for (const auto &building: buildings) {
                    if (building.GetPosition() == current_pos) {
                        std::cout << GetCharFor(building) << " ";
                        building_found = true;
                        break;
                    }
                }
                if (building_found) continue;

                bool tower_found = false;
                for (const auto &tower: towers) {
                    if (tower.GetPosition() == current_pos) {
                        std::cout << GetCharFor(tower) << " ";
                        tower_found = true;
                        break;
                    }
                }
                if (tower_found) continue;

                bool trap_found = false;
                for (const auto &trap: traps) {
                    if (trap.GetPosition() == current_pos) {
                        std::cout << GetCharFor(trap) << " ";
                        trap_found = true;
                        break;
                    }
                }
                if (trap_found) continue;

                const Cell &cell = field.GetCell(x, y);
                std::cout << GetCharFor(cell.GetType()) << " ";
            }
            std::cout << "\n";
        }
        std::cout <<
                "\nЛегенда: P - Игрок, E - Враг, A - Союзник, B - Здание, W - Башня, T - Ловушка, # - Непроходимо, ~ - Замедление, . - Обычная клетка\n";
    }

    [[nodiscard]] char ConsoleRenderer::GetCharFor(const Player &) const {
        return 'P';
    }

    [[nodiscard]] char ConsoleRenderer::GetCharFor(const Enemy &) const {
        return 'E';
    }

    [[nodiscard]] char ConsoleRenderer::GetCharFor(const EnemyBuilding &) const {
        return 'B';
    }

    [[nodiscard]] char ConsoleRenderer::GetCharFor(const Trap &) const {
        return 'T';
    }

    [[nodiscard]] char ConsoleRenderer::GetCharFor(const Ally &) const {
        return 'A';
    }

    [[nodiscard]] char ConsoleRenderer::GetCharFor(const EnemyTower &) const {
        return 'W';
    }

    [[nodiscard]] char ConsoleRenderer::GetCharFor(CellType cell_type) const {
        switch (cell_type) {
            case CellType::kNormal:
                return '.';
            case CellType::kImpassable:
                return '#';
            case CellType::kSlowing:
                return '~';
            default:
                return '?';
        }
    }

    [[nodiscard]] std::optional<Position> ConsoleRenderer::SelectTarget(
        const Game &game,
        const Position &player_pos,
        int range,
        int area_size,
        SpellType spell_type) const {
        Position cursor_pos = player_pos;
        bool selecting = true;
        std::optional<Position> result = std::nullopt;

        while (selecting) {
            RenderTargetSelectionMap(game, cursor_pos, player_pos, range, area_size, spell_type);

            int input = GetInput();

            size_t new_x = cursor_pos.x;
            size_t new_y = cursor_pos.y;

            switch (input) {
                case 'w':
                case 'W':
                    if (cursor_pos.y > 0) new_y--;
                    break;
                case 's':
                case 'S':
                    if (cursor_pos.y < game.GetField().GetHeight() - 1) new_y++;
                    break;
                case 'a':
                case 'A':
                    if (cursor_pos.x > 0) new_x--;
                    break;
                case 'd':
                case 'D':
                    if (cursor_pos.x < game.GetField().GetWidth() - 1) new_x++;
                    break;
                case ' ':
                case '\r':
                case '\n':
                    if (IsInRange(player_pos, cursor_pos, range)) {
                        result = cursor_pos;
                        selecting = false;
                    }
                    break;
                case 'q':
                case 'Q':
                case 27:  // ESC
                    result = std::nullopt;
                    selecting = false;
                    break;
                default:
                    break;
            }

            cursor_pos.x = new_x;
            cursor_pos.y = new_y;
        }
        
        return result;
    }

    void ConsoleRenderer::RenderTargetSelectionMap(
        const Game &game,
        const Position &cursor_pos,
        const Position &player_pos,
        int range,
        int area_size,
        SpellType spell_type) const {
        ClearScreen();

        std::cout << "╔═══════════════════════════════════════════════════════════════════╗\n";
        std::cout << "║                        ВЫБОР ЦЕЛИ ЗАКЛИНАНИЯ                      ║\n";
        std::cout << "╠═══════════════════════════════════════════════════════════════════╣\n";
        std::cout << "║  WASD - перемещение курсора, Пробел/Enter - применить             ║\n";
        std::cout << "║  Q/ESC - отменить                                                 ║\n";
        std::cout << "╚═══════════════════════════════════════════════════════════════════╝\n\n";

        const GameField &field = game.GetField();
        const auto &enemies = game.GetEnemies();
        const auto &buildings = game.GetBuildings();
        const auto &traps = game.GetTraps();
        const auto &allies = game.GetAllies();
        const auto &towers = game.GetTowers();

        bool in_range = IsInRange(player_pos, cursor_pos, range);
        auto affected_positions = GetAffectedPositions(cursor_pos, area_size, field);

        // Проверяем можно ли применить заклинание на текущую позицию курсора
        bool can_cast = in_range;
        std::string reason = "";

        if (spell_type == SpellType::kTrap && in_range) {
            const Cell &cell = field.GetCell(cursor_pos.x, cursor_pos.y);
            if (cell.GetType() != CellType::kNormal) {
                can_cast = false;
                if (cell.GetType() == CellType::kImpassable) {
                    reason = "Нельзя поставить ловушку на стену!";
                } else if (cell.GetType() == CellType::kSlowing) {
                    reason = "Нельзя поставить ловушку на замедляющую клетку!";
                }
            }

            if (can_cast) {
                for (const auto &trap: traps) {
                    if (trap.GetPosition() == cursor_pos) {
                        can_cast = false;
                        reason = "Здесь уже есть ловушка!";
                        break;
                    }
                }
            }

            if (can_cast) {
                for (const auto &enemy: enemies) {
                    if (enemy.GetPosition() == cursor_pos && enemy.IsAlive()) {
                        can_cast = false;
                        reason = "Здесь стоит враг!";
                        break;
                    }
                }
            }

            if (can_cast) {
                for (const auto &ally: allies) {
                    if (ally.GetPosition() == cursor_pos && ally.IsAlive()) {
                        can_cast = false;
                        reason = "Здесь стоит союзник!";
                        break;
                    }
                }
            }

            if (can_cast && cursor_pos == player_pos) {
                can_cast = false;
                reason = "Нельзя поставить ловушку на себя!";
            }
        }

        std::cout << "=== Карта (Радиус: " << range << ", Область: " << area_size << "x" << area_size << ") ===\n";

        for (size_t y = 0; y < field.GetHeight(); ++y) {
            for (size_t x = 0; x < field.GetWidth(); ++x) {
                Position current_pos(x, y);

                bool is_cursor = (cursor_pos == current_pos);

                bool is_affected = false;
                for (const auto &pos: affected_positions) {
                    if (pos == current_pos) {
                        is_affected = true;
                        break;
                    }
                }

                char display_char = '.';

                if (player_pos == current_pos) {
                    display_char = GetCharFor(game.GetPlayer());
                } else {
                    bool found = false;

                    for (const auto &enemy: enemies) {
                        if (enemy.GetPosition() == current_pos && enemy.IsAlive()) {
                            display_char = GetCharFor(enemy);
                            found = true;
                            break;
                        }
                    }

                    if (!found) {
                        for (const auto &ally: allies) {
                            if (ally.GetPosition() == current_pos && ally.IsAlive()) {
                                display_char = GetCharFor(ally);
                                found = true;
                                break;
                            }
                        }
                    }

                    if (!found) {
                        for (const auto &building: buildings) {
                            if (building.GetPosition() == current_pos) {
                                display_char = GetCharFor(building);
                                found = true;
                                break;
                            }
                        }
                    }

                    if (!found) {
                        for (const auto &tower: towers) {
                            if (tower.GetPosition() == current_pos) {
                                display_char = GetCharFor(tower);
                                found = true;
                                break;
                            }
                        }
                    }

                    if (!found) {
                        for (const auto &trap: traps) {
                            if (trap.GetPosition() == current_pos) {
                                display_char = GetCharFor(trap);
                                found = true;
                                break;
                            }
                        }
                    }

                    if (!found) {
                        const Cell &cell = field.GetCell(x, y);
                        display_char = GetCharFor(cell.GetType());
                    }
                }

                if (is_cursor) {
                    std::cout << "\033[1;33m[" << display_char << "]\033[0m";
                } else if (is_affected && in_range) {
                    std::cout << "\033[1;31m{" << display_char << "}\033[0m";
                } else if (!in_range && is_cursor) {
                    std::cout << "\033[1;31m[" << display_char << "]\033[0m";
                } else {
                    std::cout << " " << display_char << " ";
                }
            }
            std::cout << "\n";
        }

        std::cout << "\nСтатус: ";
        if (!in_range) {
            std::cout << "\033[1;31mЦель ВНЕ радиуса действия!\033[0m";
        } else if (can_cast) {
            std::cout << "\033[1;32mМожно применить заклинание\033[0m";
        } else {
            std::cout << "\033[1;31m" << reason << "\033[0m";
        }
        std::cout << "\n";

        std::cout << "\nЛегенда: [X] - Курсор, {X} - Область поражения\n";
    }

    [[nodiscard]] bool ConsoleRenderer::IsInRange(
        const Position &from,
        const Position &to,
        int range) const {
        int dx = std::abs(static_cast<int>(to.x) - static_cast<int>(from.x));
        int dy = std::abs(static_cast<int>(to.y) - static_cast<int>(from.y));

        // Для диапазона 1 используем чебышёвское расстояние (8 соседних клеток)
        if (range == 1) {
            return dx <= 1 && dy <= 1;
        }
        // Для остальных - евклидово
        double distance = std::sqrt(dx * dx + dy * dy);
        return distance <= static_cast<double>(range);
    }

    [[nodiscard]] std::vector<Position> ConsoleRenderer::GetAffectedPositions(
        const Position &center,
        int area_size,
        const GameField &field) const {
        std::vector<Position> positions;

        if (area_size == 1) {
            positions.push_back(center);
            return positions;
        }

        int half_size = area_size / 2;

        for (int dy = -half_size; dy <= half_size; ++dy) {
            for (int dx = -half_size; dx <= half_size; ++dx) {
                int new_x = static_cast<int>(center.x) + dx;
                int new_y = static_cast<int>(center.y) + dy;

                if (new_x >= 0 && new_y >= 0 &&
                    new_x < static_cast<int>(field.GetWidth()) &&
                    new_y < static_cast<int>(field.GetHeight())) {
                    positions.emplace_back(static_cast<size_t>(new_x),
                                           static_cast<size_t>(new_y));
                }
            }
        }

        return positions;
    }

    [[nodiscard]] int ConsoleRenderer::GetInput() const {
#ifdef _WIN32
        return _getch();
#else
        struct termios oldt, newt;
        int ch;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return ch;
#endif
    }
}
