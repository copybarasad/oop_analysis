#pragma once
#include "Pos.hpp"

// Структура команды для передачи между компонентами системы ввода
struct Command {
    // Тип команды
    enum class Type {
        NONE,               // Пустая команда
        MOVE,               // Движение
        ATTACK,             // Атака
        SWITCH_COMBAT_MODE, // Смена режима боя
        QUIT,               // Выход
        CAST_SPELL,         // Применение заклинания
        MENU_SELECT,        // Выбор в меню
        SAVE_GAME,          // Сохранение игры
        MAIN_MENU           // Возврат в главное меню
    };
    
    Type type;              // Тип команды
    Pos direction;          // Направление для движения/атаки
    int spellIndex = -1;    // Индекс заклинания
    int menuIndex = -1;     // Индекс пункта меню
};
