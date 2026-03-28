#include "RenderSystem.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <cstdlib>
#endif

RenderSystem::RenderSystem() = default;

void RenderSystem::render(const Field &field,
                          const Player &player,
                          const std::vector<Enemy> &enemies,
                          const std::vector<EnemyBuilding> &buildings,
                          const std::vector<EnemyTower> &towers,
                          const std::vector<Trap> &traps,
                          const std::vector<Ally> &allies,
                          int enemiesKilled,
                          int scoreTarget,
                          int currentLevel) const {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    int playerX, playerY;
    player.getPosition(playerX, playerY);

    std::cout << "=== ПОДЗЕМЕЛЬЕ === (уровень " << currentLevel << ")\n";
    std::cout << "Здоровье: " << player.getHealth() << " | "
            << "Очки: " << player.getScore() << "/" << scoreTarget << " | "
            << "Оружие: " << (player.getBattleType() == BattleType::MELEE ? "Ближний" : "Дальний")
            << " (Урон: " << (player.getBattleType() == BattleType::MELEE
                                  ? player.getMeleeDamage()
                                  : player.getRangedDamage()) << ")\n";
    std::cout << "Позиция: (" << playerX << ", " << playerY << ") | "
            << "Враги: " << enemies.size() << " | "
            << "Здания: " << buildings.size() << " | "
            << "Башни: " << towers.size() << " | "
            << "Ловушки: " << traps.size() << " | "
            << "Заклинания: " << player.getHand().getSize() << "/" << player.getHand().getMaxSize()
            << " | Убийств: " << enemiesKilled << "\n\n";

    std::cout <<
            "Обозначения: P=Игрок, E=Враг, A=Союзник, B=Здание, W=Башня, T=Ловушка, #=Стена, ~=Замедление, .=Пусто\n";
    if (controls) {
        char up = controls->keyFor(CommandType::MOVE_UP);
        char down = controls->keyFor(CommandType::MOVE_DOWN);
        char left = controls->keyFor(CommandType::MOVE_LEFT);
        char right = controls->keyFor(CommandType::MOVE_RIGHT);
        char attack = controls->keyFor(CommandType::ATTACK);
        char switchW = controls->keyFor(CommandType::SWITCH_WEAPON);
        char cast = controls->keyFor(CommandType::CAST_SPELL);
        char save = controls->keyFor(CommandType::SAVE_GAME);
        char load = controls->keyFor(CommandType::LOAD_GAME);
        char exit = controls->keyFor(CommandType::EXIT);
        std::cout << "Управление: "
                << up << "/" << down << "/" << left << "/" << right << " ход, "
                << attack << " атака, "
                << switchW << " оружие, "
                << cast << " заклинание, "
                << save << " сохранить, "
                << load << " загрузить, "
                << exit << " выход\n";
    } else {
        std::cout << "Управление: смотрите текущие привязки (controls.cfg)\n";
    }
    std::cout << "Цель: достигнуть нужного числа очков на уровне.\n\n";

    std::cout << "  ";
    for (int x = 0; x < field.getWidth(); ++x) {
        std::cout << x % 10 << " ";
    }
    std::cout << "\n";

    for (int y = 0; y < field.getHeight(); ++y) {
        std::cout << y % 10 << " ";
        for (int x = 0; x < field.getWidth(); ++x) {
            if (x == playerX && y == playerY) {
                std::cout << "P ";
            } else {
                bool rendered = false;

                for (const auto &enemy: enemies) {
                    int enemyX, enemyY;
                    enemy.getPosition(enemyX, enemyY);
                    if (x == enemyX && y == enemyY && enemy.isAlive()) {
                        std::cout << "E ";
                        rendered = true;
                        break;
                    }
                }
                if (rendered) continue;

                for (const auto &ally: allies) {
                    int allyX, allyY;
                    ally.getPosition(allyX, allyY);
                    if (x == allyX && y == allyY && ally.isAlive()) {
                        std::cout << "A ";
                        rendered = true;
                        break;
                    }
                }
                if (rendered) continue;

                for (const auto &tower: towers) {
                    int towerX, towerY;
                    tower.getPosition(towerX, towerY);
                    if (x == towerX && y == towerY) {
                        std::cout << "W ";
                        rendered = true;
                        break;
                    }
                }
                if (rendered) continue;

                for (const auto &building: buildings) {
                    int buildingX, buildingY;
                    building.getPosition(buildingX, buildingY);
                    if (x == buildingX && y == buildingY) {
                        std::cout << "B ";
                        rendered = true;
                        break;
                    }
                }
                if (rendered) continue;

                for (const auto &trap: traps) {
                    if (x == trap.getX() && y == trap.getY() && trap.isActive()) {
                        std::cout << "T ";
                        rendered = true;
                        break;
                    }
                }
                if (rendered) continue;

                const Cell &cell = field.getCell(x, y);
                switch (cell.getType()) {
                    case CellType::OBSTACLE: std::cout << "# ";
                        break;
                    case CellType::SLOW: std::cout << "~ ";
                        break;
                    default: std::cout << ". ";
                        break;
                }
            }
        }
        std::cout << std::endl;
    }

    if (consoleLogEnabled) {
        std::cout << "\n=== ЖУРНАЛ ===\n";
        if (messageLog.empty()) {
            std::cout << "> Нет сообщений\n";
        } else {
            size_t totalMessages = messageLog.size();
            size_t newMessagesCount = currentTurnMessages.size();
            size_t startOfNewMessages = totalMessages - newMessagesCount;

            for (size_t i = 0; i < messageLog.size(); ++i) {
                if (i >= startOfNewMessages) {
                    std::cout << ">> " << messageLog[i] << "\n";
                } else {
                    std::cout << "> " << messageLog[i] << "\n";
                }
            }
        }
    }

    std::cout << "\nКоманда: ";
    std::cout.flush();
}

void RenderSystem::addMessage(const std::string &message) const {
    messageLog.push_back(message);
    currentTurnMessages.push_back(message);
    if (sink) {
        sink->write(message);
    }

    if (messageLog.size() > MAX_MESSAGES) {
        messageLog.pop_front();
    }
}

void RenderSystem::clearCurrentTurnMessages() {
    currentTurnMessages.clear();
}

void RenderSystem::promptInput(const std::string &text) const {
    std::cout << text;
    std::cout.flush();
}

void RenderSystem::displaySpellSelection(const Hand &hand) const {
    hand.display();
    std::cout << "Выберите заклинание (0-" << hand.getSize() - 1 << "): ";
}

void RenderSystem::setLogSink(ILogSink *sinkPtr) {
    sink = sinkPtr;
}

void RenderSystem::setConsoleLogEnabled(bool enabled) {
    consoleLogEnabled = enabled;
}

void RenderSystem::setControls(const ControlMapping *mapping) {
    controls = mapping;
}
