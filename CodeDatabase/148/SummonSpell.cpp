#include "SummonSpell.h"
#include "SpellHand.h"
#include "Game.h"
#include "Ally.h"
#include <iostream>
#include <vector>


/**
 * @file SummonSpell.cpp
 * @brief Реализация заклинания призыва союзников
 */


/**
 * @brief Конструктор заклинания призыва
 * @param count Количество призываемых союзников (по умолчанию 1)
 * 
 * Инициализирует заклинание с динамическим описанием.
 */
SummonSpell::SummonSpell(int count)
    : Spell("Summon Ally", "Summons " + std::to_string(count) + " ally to fight for you", 0),  // ← добавлен 0
      summonCount(count) {
}



/**
 * @brief Применить заклинание (призвать союзников)
 * @param game Указатель на игру
 * @return true если хотя бы один союзник был призван
 * 
 * Процесс:
 * 1. Получаем позицию игрока
 * 2. Применяем усиления если активны
 * 3. Ищем свободные соседние клетки
 * 4. Создаём союзников на найденные позиции
 */
bool SummonSpell::cast(Game* game) {
    if (!game) {
        return false;
    }
    
    const Player* player = game->getPlayer();  ///< Получаем указатель на игрока
    auto [playerX, playerY] = player->getPosition();  ///< Распаковываем позицию игрока
    
    std::cout << "\n=== Summon Spell ===\n";
    std::cout << "Summoning " << summonCount << " ally/allies near player position.\n";
    
    // Применяем усиление если есть
    int finalCount = summonCount;  ///< Начальное количество союзников
    SpellHand* hand = game->getPlayerSpellHand();  ///< Получаем руку игрока
    if (hand && hand->getBoostStacks() > 0) {  ///< Если есть активные усиления
        int boostAmount = hand->getBoostStacks();  ///< Количество стаков = количество дополнительных союзников
        finalCount += boostAmount;  ///< Добавляем бонусных союзников
        std::cout << "Boost applied! Summoning " << boostAmount << " additional ally/allies (total: " 
                  << finalCount << ")\n";  ///< Уведомляем об усилении
    }
    
    int successCount = 0;  ///< Счётчик успешно призванных союзников
    
    // Пытаемся призвать союзников
    for (int i = 0; i < finalCount; ++i) {  ///< Для каждого союзника
        int offsetX, offsetY;  ///< Смещение от позиции игрока
        if (findFreeAdjacentCell(game, playerX, playerY, offsetX, offsetY)) {  ///< Если нашли свободную клетку
            int allyX = playerX + offsetX;  ///< Вычисляем X-позицию союзника
            int allyY = playerY + offsetY;  ///< Вычисляем Y-позицию союзника
            
            game->addAlly(std::make_unique<Ally>(allyX, allyY));  ///< Добавляем союзника в игру
            std::cout << "Summoned ally at (" << allyX << ", " << allyY << ")\n";  ///< Уведомляем о призыве
            successCount++;  ///< Увеличиваем счётчик
        } else {
            std::cout << "No free space to summon ally #" << (i + 1) << "\n";  ///< Нет свободной клетки
        }
    }
    
    if (successCount == 0) {  ///< Если ни одного союзника не призвали
        std::cout << "Failed to summon any allies! No free space around player.\n";  ///< Ошибка
        return false;
    }
    
    std::cout << "Successfully summoned " << successCount << " ally/allies!\n";  ///< Успех
    return true;
}


/**
 * @brief Найти свободную соседнюю клетку
 * @param game Указатель на игру
 * @param centerX X-координата центра поиска
 * @param centerY Y-координата центра поиска
 * @param offsetX Выходной параметр: смещение по X
 * @param offsetY Выходной параметр: смещение по Y
 * @return true если свободная клетка найдена
 * 
 * Проверяет все 8 соседних клеток и возвращает первую свободную.
 */
bool SummonSpell::findFreeAdjacentCell(Game* game, int centerX, int centerY, int& offsetX, int& offsetY) {
    // Проверяем все соседние клетки (8 направлений Муура)
    std::vector<std::pair<int, int>> offsets = {
        {-1, -1}, {0, -1}, {1, -1},  ///< Верхний ряд
        {-1,  0},          {1,  0},  ///< Средний ряд (исключая центр)
        {-1,  1}, {0,  1}, {1,  1}   ///< Нижний ряд
    };
    
    const GameField* field = game->getField();  ///< Получаем указатель на поле
    
    for (const auto& [dx, dy] : offsets) {  ///< Для каждого смещения
        int checkX = centerX + dx;  ///< Вычисляем X-координату для проверки
        int checkY = centerY + dy;  ///< Вычисляем Y-координату для проверки
        
        // Проверяем границы поля
        if (checkX < 0 || checkX >= field->getWidth() || 
            checkY < 0 || checkY >= field->getHeight()) {  ///< За границами поля
            continue;
        }
        
        bool occupied = false;  ///< Флаг: клетка занята
        
        // Проверяем позицию игрока
        auto [px, py] = game->getPlayer()->getPosition();  ///< Позиция игрока
        if (checkX == px && checkY == py) {  ///< Занято игроком
            occupied = true;
        }
        
        // Проверяем врагов
        if (!occupied) {  ///< Если ещё не занято
            for (const auto& enemy : game->getEnemies()) {  ///< Для каждого врага
                if (!enemy->isAlive()) continue;  ///< Пропускаем мёртвых
                auto [ex, ey] = enemy->getPosition();  ///< Позиция врага
                if (checkX == ex && checkY == ey) {  ///< Занято врагом
                    occupied = true;
                    break;
                }
            }
        }
        
        // Проверяем башни
        if (!occupied) {  ///< Если ещё не занято
            for (const auto& tower : game->getTowers()) {  ///< Для каждой башни
                if (!tower->isAlive()) continue;  ///< Пропускаем разрушенные
                auto [tx, ty] = tower->getPosition();  ///< Позиция башни
                if (checkX == tx && checkY == ty) {  ///< Занято башней
                    occupied = true;
                    break;
                }
            }
        }
        
        // Проверяем других союзников
        if (!occupied) {  ///< Если ещё не занято
            for (const auto& ally : game->getAllies()) {  ///< Для каждого союзника
                if (!ally->isAlive()) continue;  ///< Пропускаем мёртвых
                auto [ax, ay] = ally->getPosition();  ///< Позиция союзника
                if (checkX == ax && checkY == ay) {  ///< Занято союзником
                    occupied = true;
                    break;
                }
            }
        }
        
        if (!occupied) {  ///< Если клетка свободна
            offsetX = dx;  ///< Сохраняем смещение X
            offsetY = dy;  ///< Сохраняем смещение Y
            return true;  ///< Нашли свободную клетку
        }
    }
    
    return false;  ///< Свободных клеток не найдено
}




