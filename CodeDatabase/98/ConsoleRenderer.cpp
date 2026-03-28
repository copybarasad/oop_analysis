#include "ConsoleRenderer.h"
#include <iostream>
#include <limits>

void ConsoleRenderer::RenderField(const Field& field, int playerX, int playerY, const std::vector<std::pair<int,int>>& enemyPositions) {
    std::cout << "\n=== ИГРОВОЕ ПОЛЕ ===" << std::endl;
    for(int y = 0; y < field.GetHeight(); y++) {
        for(int x = 0; x < field.GetWidth(); x++) {
            bool printed = false;
            
            if (x == playerX && y == playerY) {
                std::cout << "P";
                printed = true;
            }

            if (!printed) {
                for (const auto& pos : enemyPositions) {
                    if (pos.first == x && pos.second == y) {
                        std::cout << "E";
                        printed = true;
                        break;
                    }
                }
            }

            if (!printed) {
                std::cout << (field.GetCell(x, y).IsBlocked() ? '#' : '.');
            }
        }
        std::cout << '\n';
    }
    
}

void ConsoleRenderer::RenderMenu(const Player& player, const Hand& hand, int playerX, int playerY) {
    ShowSeparator();
    std::cout << "=== ДОСТУПНЫЕ КОМАНДЫ ===" << std::endl;
    std::cout << "Движение: w(вверх), s(вниз), a(влево), d(вправо)" << std::endl;
    std::cout << "f - атака ближайшего врага" << std::endl;
    std::cout << "r - сменить тип атаки" << std::endl;
    std::cout << "e - поиск врагов поблизости" << std::endl;
    std::cout << "v N - использовать заклинание (N - номер)" << std::endl;
    std::cout << "m - показать характеристики" << std::endl;
    std::cout << "p - показать поле" << std::endl;
    std::cout << "S - сохранить игру" << std::endl;
    std::cout << "L - загрузить игру" << std::endl;
    std::cout << "q - выйти" << std::endl;
    
    std::string spellsInfo = hand.GetSpellsInfo();
    if (!spellsInfo.empty()) {
        std::cout << "\nТекущие заклинания:\n" << spellsInfo;
    } else {
        std::cout << "\nЗаклинаний нет" << std::endl;
    }
    
    std::cout << "\nТекущее состояние:" << std::endl;
    std::cout << "Здоровье: " << player.GetHealth() << "/" << player.GetMaxHealth();
    std::cout << " | Тип атаки: " << (player.GetAttackType() == AttackType::MELEE ? "ближний" : "дальний");
    std::cout << " | Убийств: " << player.GetMurder() << std::endl;
    std::cout << "твои координаты: (" << playerX << ", " << playerY << ")" << std::endl;
    
    std::cout << "\nВыбери команду: ";
}

void ConsoleRenderer::RenderPlayerStats(const Player& player, const Hand& hand) {
    ShowSeparator();
    std::cout << "=== ХАРАКТЕРИСТИКИ ИГРОКА ===" << std::endl;
    std::cout << "Здоровье: " << player.GetHealth() << "/" << player.GetMaxHealth() << std::endl;
    std::cout << "Скорость: " << player.GetSpeed() << std::endl;
    std::cout << "Ближний урон: " << player.GetMeleeDamage() << std::endl;
    std::cout << "Дальний урон: " << player.GetRangedDamage() << std::endl;
    std::cout << "Тип атаки: " << (player.GetAttackType() == AttackType::MELEE ? "ближний" : "дальний") << std::endl;
    std::cout << "Убийств: " << player.GetMurder() << std::endl;
    
    std::string spellsInfo = hand.GetSpellsInfo();
    if (!spellsInfo.empty()) {
        std::cout << "\nЗаклинания:\n" << spellsInfo;
    } else {
        std::cout << "\nЗаклинаний нет" << std::endl;
    }
}

void ConsoleRenderer::RenderAttackResult(const AttackResult& result) {
    ShowSeparator();
    std::cout << "=== РЕЗУЛЬТАТ БОЯ ===" << std::endl;
    std::cout << result.message << std::endl;
    
    if (result.playerDefeated) {
        std::cout << "\nТы проиграла" << std::endl;
    } else if (result.enemyDefeated) {
        std::cout << "\nТы выиграла!" << std::endl;
    }
}

void ConsoleRenderer::RenderSearchResults(const std::vector<EnemyInfo>& enemies, int playerX, int playerY) {
    ShowSeparator();
    std::cout << "=== ПОИСК ВРАГОВ ===" << std::endl;
    std::cout << "Твои координаты: (" << playerX << ", " << playerY << ")" << std::endl;
    std::cout << "Поиск в радиусе 6 клеток..." << std::endl;
    
    if (enemies.empty()) {
        std::cout << "Поблизости нет врагов" << std::endl;
    } else {
        std::cout << "Найдено врагов: " << enemies.size() << std::endl;
        for (const auto& enemy : enemies) {
            RenderEnemyInfo(enemy);
        }
    }
}

void ConsoleRenderer::RenderEnemyInfo(const EnemyInfo& enemy) {
    std::cout << "Враг " << enemy.id << ":" << std::endl;
    std::cout << "  Координаты: (" << enemy.x << ", " << enemy.y << ")" << std::endl;
    std::cout << "  Расстояние: " << enemy.distance << " клеток" << std::endl;
    std::cout << "  Здоровье: " << enemy.health << "/100" << std::endl;
}

void ConsoleRenderer::RenderEnemyAttacks(const std::vector<std::string>& attacks) {
    if (!attacks.empty()) {
        ShowSeparator();
        std::cout << "=== ХОД ВРАГОВ ===" << std::endl;
        for (const auto& attack : attacks) {
            std::cout << attack << std::endl;
        }
    }
}

void ConsoleRenderer::RenderGameMessage(const std::string& message) {
    if (!message.empty()) {
        ShowSeparator();
        std::cout << "=== СОБЫТИЕ ===" << std::endl;
        std::cout << message << std::endl;
    }
}

void ConsoleRenderer::RenderUpgradeMenu() {
    ShowSeparator();
    std::cout << "=== ПРОКАЧКА ПЕРСОНАЖА ===" << std::endl;
    std::cout << "Выбери улучшение:" << std::endl;
    std::cout << "1 - Увеличить максимальное здоровье (+20)" << std::endl;
    std::cout << "2 - Увеличить ближний урон (+5)" << std::endl;
    std::cout << "3 - Увеличить дальний урон (+5)" << std::endl;
    std::cout << "4 - Увеличить скорость (+1)" << std::endl;
    std::cout << "Ваш выбор: ";
}

void ConsoleRenderer::RenderSaveLoadMessage(const std::string& message) {
    ShowSeparator();
    std::cout << "=== СОХРАНЕНИЕ/ЗАГРУЗКА ===" << std::endl;
    std::cout << message << std::endl;
}

void ConsoleRenderer::RenderGameOver(bool playerWon) {
    ShowSeparator();
    if (playerWon) {
        std::cout << "=== ПОБЕДА! ===" << std::endl;
        std::cout << "ты прошла уровень!!" << std::endl;
    } else {
        std::cout << "=== ИГРА ОКОНЧЕНА ===" << std::endl;
        std::cout << "ты проиграла!" << std::endl;
    }
}



void ConsoleRenderer::ShowSeparator() {
    std::cout << "\n" << std::string(50, '=') << "\n";
}

// В ConsoleRenderer.cpp добавьте:
void ConsoleRenderer::RenderSpellResult(const SpellResult& result) {
    ShowSeparator();
    std::cout << "=== РЕЗУЛЬТАТ ЗАКЛИНАНИЯ ===" << std::endl;
    std::cout << result.message << std::endl;
    
    if (result.enemiesHit > 0) {
        std::cout << "Затронуто врагов: " << result.enemiesHit << std::endl;
    }
}