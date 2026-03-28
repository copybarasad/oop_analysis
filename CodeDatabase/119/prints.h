#ifndef PRINTS_H
#define PRINTS_H

#include <iostream>
#include <string>
#include <fstream> 
#include "log.h" 

class Prints
{
private:
    inline static Logger* getOut() {
        return Log::getStream();
    }

    // Строковые константы
    inline static const std::string tutorial_string = "Для перемещения по карте введите: \"W\", \"A\", \"S\", \"D\". Чтобы сменить тип атаки введите \"Q\", для дальней атаки введите \"F\".  Для выхода введите \"quit\".\n";
    inline static const std::string incorrect_mode_string = "Сначала переключите режим атаки!\n";
    inline static const std::string incorrect_input_string = "Некорректный ввод!\n";
    inline static const std::string game_over_string = "Игра окончена!\n";

    inline static const std::string area_use_string = "Применение заклинания урона по площади\n";
    inline static const std::string out_of_range_string = "Игрок находится вне поля!\n";
    inline static const std::string spell_upgrade_string = "Какое из ваших текущих заклинаний вы хотите усилить?\n";

    inline static const std::string spell_miss_string = "Заклинание было использовано, но урона не нанесло\n";
    inline static const std::string hand_is_full_string = "В руке нет места для нового заклинания!\n"; 
    inline static const std::string hand_is_empty_string = "Рука пуста.\n";
    inline static const std::string incorrect_spell_string = "Неверный номер заклинания!\n";

    inline static const std::string no_xp_string = "Недостаточно Xp для покупки!\n";

    inline static const std::string impassable_cell_string = "Непроходимая клетка!\n";
    inline static const std::string tower_destroyed_string = "Вражеская башня была разрушена!\n";

    inline static const std::string changed_on_ranged_string = "Игрок переключился на дальний бой!\n";
    inline static const std::string changed_on_melee_string = "Игрок переключился на ближний бой!\n";
    inline static const std::string range_attack_string = "Игрок атаковал врага на расстоянии!\n";
    inline static const std::string range_miss_string = "Промах!\n";

    inline static const std::string enemy_trapped_string = "Враг попал в ловушку!\n";
    inline static const std::string enemy_died_from_trap_string = "Враг был убит ловушкой!\n";
    inline static const std::string trap_destroyed_string = "Невозможно установить ловушку, ловушка сломалась!\n";
    inline static const std::string trap_placed_string = "Ловушка была успешно установлена\n";
    inline static const std::string getting_ready_string = "Подготовка к следующему испытанию...\n";
    
    inline static const std::string prompt_for_upgrade_string = "Введите номер заклинания для улучшения или 'back' для возврата: ";
    inline static const std::string load_game_prompt_string = "Введите load, чтобы Загрузить Игру.\n";
    inline static const std::string start_game_prompt_string = "Введите start, чтобы начать Новую Игру.\n";
    inline static const std::string starting_new_game_string = "Начинаем новую игру...\n";
    inline static const std::string game_saved_string = "Игра сохранена.\n";
    inline static const std::string game_loaded_string = "Игра загружена.\n";

public:

    static void log_error(const std::string& message)
    {
        *getOut() << "[ERROR]: " << message << std::endl;
        if (Log::isFileOutput()) {
            getOut()->flush();
        }
    }

    static void print_tutorial_string()
    {
        *getOut() << tutorial_string;
    }

    static void print_incorrect_mode_string()
    {
        *getOut() << incorrect_mode_string;
    }

    static void print_incorrect_input_string()
    {
        *getOut() << incorrect_input_string;
    }

    static void print_game_over_string()
    {
        *getOut() << game_over_string;
    }

    static void print_out_of_range_string()
    {
        *getOut() << out_of_range_string;
    }

    static void print_spell_upgrade_string()
    {
        *getOut() << spell_upgrade_string;
    }

    static void print_area_use_string(){
        *getOut() << area_use_string;
    }

    static void print_spell_miss_string()
    {
        *getOut() << spell_miss_string;
    }

    static void print_hand_is_full_string()
    {
        *getOut() << hand_is_full_string;
    }

    static void print_hand_is_empty_string()
    {
        *getOut() << hand_is_empty_string;
    }

    static void print_incorrect_spell_string()
    {
        *getOut() << incorrect_spell_string;
    }

    static void print_no_xp_string()
    {
        *getOut() << no_xp_string;
    }

    static void print_impassable_cell_string()
    {
        *getOut() << impassable_cell_string;
    }

    static void print_tower_destroyed_string()
    {
        *getOut() << tower_destroyed_string;
    }

    static void print_changed_on_ranged_string()
    {
        *getOut() << changed_on_ranged_string;
    }

    static void print_changed_on_melee_string()
    {
        *getOut() << changed_on_melee_string;
    }

    static void print_range_attack_string()
    {
        *getOut() << range_attack_string;
    }

    static void print_range_miss_string()
    {
        *getOut() << range_miss_string;
    }

    static void print_enemy_trapped_string()
    {
        *getOut() << enemy_trapped_string;
    }

    static void print_enemy_died_from_trap_string()
    {
        *getOut() << enemy_died_from_trap_string;
    }

    static void print_trap_destroyed_string()
    {
        *getOut() << trap_destroyed_string;
    }

    static void print_trap_placed_string()
    {
        *getOut() << trap_placed_string;
    }

    static void print_getting_ready_string()
    {
        *getOut() << getting_ready_string;
    }

    static void print_game_status(int playerHP, int playerXP, int enemyHP, int enemy2HP, int enemyTowerHP)
    {
        *getOut() << "HP игрока: " << playerHP << std::endl;
        *getOut() << "Накопленные XP: " << playerXP << std::endl;
        *getOut() << "HP врага: " << enemyHP << std::endl;
        *getOut() << "HP врага 2: " << enemy2HP << std::endl;
        *getOut() << "HP вражеской башни: " << enemyTowerHP << std::endl;
        *getOut() << "Новое заклинание стоит 5 XP, чтобы его получить, введите M." << std::endl
                  << "А что бы его использовать введите U." << std::endl;
    }

    static void print_take_damage(int dmg, std::string name)
    {
        *getOut() << "Нанесено " << dmg << " урона " << name << "\n";
    }

    static void print_player_movement(int x, int y) {
        *getOut() << "Игрок переместился на координаты (" << x << ", " << y << ")." << std::endl;
    }

    static void print_enemy_dead(std::string name)
    {
        *getOut() << name << " погиб!\n";
    }

    static void print_entity_killed_other(std::string name, std::string other_name)
    {
        *getOut() << name << " ликвидировал " << other_name << "!\n";
    }

    static void print_top_spell(int spells_size, int max_size)
    {
        *getOut() << "--- Заклинания в руке (" << spells_size << "/" << max_size << ") ---" << std::endl;
    }

    static void print_spell_add(std::string spell_name)
    {
        *getOut() << "В руку добавлено новое заклинание: " << spell_name << std::endl;
    }

    static void print_spell_cast(std::string spell_name)
    {
        *getOut() << "Игрок применяет заклинание '" << spell_name << "'..." << std::endl;
    }

    static void print_spell_upgraded(std::string spell_name)
    {
        *getOut() << "Заклинание '" << spell_name << "' было улучшено! " << std::endl;
    }

    static void print_spell_removed(int spellsToRemove)
    {
        *getOut() << spellsToRemove << " заклинаний было утеряно при переходе." << std::endl;
    }

    static void print_upgrade_screen_header()
    {
        *getOut() << "========================================" << std::endl;
        *getOut() << "===       ВРЕМЯ УЛУЧШЕНИЙ!           ===" << std::endl;
        *getOut() << "========================================" << std::endl;
        *getOut() << "Выберите одну из наград за прохождение уровня:" << std::endl;
    }

    static void print_upgrade_options(int currentMaxHp, int currentHandSize)
    {
        *getOut() << "\n1. Увеличить максимальное здоровье (Текущее: " << currentMaxHp << " -> " << currentMaxHp + 20 << ")" << std::endl;
        *getOut() << "2. Увеличить размер руки (Текущий: " << currentHandSize << " -> " << currentHandSize + 1 << ")" << std::endl;
        *getOut() << "3. Улучшить одно из заклинаний в руке" << std::endl;
        *getOut() << "\nВаш выбор: ";
    }
    
    static void print_starting_new_game()
    {
        *getOut() << starting_new_game_string;
    }

    static void print_game_saved()
    {
        *getOut() << game_saved_string;
    }
    static void print_game_loaded()
    {
        *getOut() << game_loaded_string;
    }

    static void print_menu_options()
    {
        *getOut() << load_game_prompt_string;
        *getOut() << start_game_prompt_string;
    }
    
    static void print_level_generation(int currentLevel)
    {
        *getOut() << "Генерация уровня " << currentLevel << "..." << std::endl;
    }
    
    static void print_prompt_for_upgrade()
    {
        *getOut() << prompt_for_upgrade_string;
    }

    static void print_level_completed(int currentLevel)
    {
        *getOut() << "Поздравляем! Уровень " << currentLevel << " пройден!" << std::endl;
    }

    // Методы ошибок исправлены: заменен 'out' на 'getOut()' и добавлена проверка Log::isFileOutput()
    static void print_load_game_error(const std::exception &e)
    {
        *getOut() << "[ERROR] Не удалось загрузить игру: " << e.what() << std::endl;
        if (Log::isFileOutput()) {
            getOut()->flush();
        }
    }
    
    static void print_save_error(const std::exception &e)
    {
        *getOut() << "[ERROR] Ошибка сохранения: " << e.what() << std::endl;
        if (Log::isFileOutput()) {
            getOut()->flush();
        }
    }

    static void print_load_error(const std::exception &e)
    {
        *getOut() << "[ERROR] Ошибка загрузки: " << e.what() << std::endl;
        if (Log::isFileOutput()) {
            getOut()->flush();
        }
    }
};

#endif