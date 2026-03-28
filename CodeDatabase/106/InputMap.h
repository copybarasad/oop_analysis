#ifndef CPPRPG_INPUTMAP_H
#define CPPRPG_INPUTMAP_H

#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>

class InputMap {
public:
    enum Action {
        UP,        // Движение вверх
        DOWN,      // Движение вниз
        LEFT,      // Движение влево
        RIGHT,     // Движение вправо
        CAST,      // Каст заклинания
        ATTACK,    // Обычная атака
        SAVE,      // Сохранение
        QUIT,      // Выход
        SPELL_0,   // Заклинание 1 (4)
        SPELL_1,   // Заклинание 2 (5)
        SPELL_2,   // Заклинание 3 (6)
        SPELL_3,   // Заклинание 4 (7)
        SPELL_4,   // Заклинание 5 (8)
        MENU_0,    // Пункт меню 1 (1)
        MENU_1,    // Пункт меню 2 (2)
        MENU_2,    // Пункт меню 3 (3)
        ACTION_COUNT // Общее количество действий
    };

    static InputMap load();

    char get(Action action) const;
    char getChar(char action) const;
    Action getAction(char input) const;
    bool isValid() const;
    void resetToDefaults();

private:
    std::unordered_map<Action, char> actionToKey;
    std::unordered_map<char, Action> keyToAction;

    bool hasConflicts() const;

    void loadDefaults();
};

extern const InputMap Controls;

#endif // CPPRPG_INPUTMAP_H