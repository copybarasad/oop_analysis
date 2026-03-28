#ifndef PRINTS_H
#define PRINTS_H

#include <iostream>
#include <string>
#include <fstream> 

class Prints
{
private:

    
    inline static std::ostream* out = &std::cout;
    
    inline static std::ofstream logFile;

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

    static void init(bool useFile) {
        if (useFile) {
            logFile.open("game_log.txt");
            if (logFile.is_open()) {
                out = &logFile; 
            } else {
                std::cerr << "Ошибка: Не удалось создать файл логов. Используется консоль.\n";
                out = &std::cout;
            }
        } else {
            out = &std::cout;
        }
    }

    static void log_error(const std::string& message)
    {
        
        *out << "[ERROR]: " << message << std::endl;
        
        
        
        if (out != &std::cout) {
            out->flush();
        }
    }

    static void print_tutorial_string()
    {
        *out << tutorial_string;
    }

    static void print_incorrect_mode_string()
    {
        *out << incorrect_mode_string;
    }

    static void print_incorrect_input_string()
    {
        *out << incorrect_input_string;
    }

    static void print_game_over_string()
    {
        *out << game_over_string;
    }

    static void print_out_of_range_string()
    {
        *out << out_of_range_string;
    }

    static void print_spell_upgrade_string()
    {
        *out << spell_upgrade_string;
    }

    static void print_area_use_string(){
        *out << area_use_string;
    }

    static void print_spell_miss_string()
    {
        *out << spell_miss_string;
    }

    static void print_hand_is_full_string()
    {
        *out << hand_is_full_string;
    }

    static void print_hand_is_empty_string()
    {
        *out << hand_is_empty_string;
    }

    static void print_incorrect_spell_string()
    {
        *out << incorrect_spell_string;
    }

    static void print_no_xp_string()
    {
        *out << no_xp_string;
    }

    static void print_impassable_cell_string()
    {
        *out << impassable_cell_string;
    }

    static void print_tower_destroyed_string()
    {
        *out << tower_destroyed_string;
    }

    static void print_changed_on_ranged_string()
    {
        *out << changed_on_ranged_string;
    }

    static void print_changed_on_melee_string()
    {
        *out << changed_on_melee_string;
    }

    static void print_range_attack_string()
    {
        *out << range_attack_string;
    }

    static void print_range_miss_string()
    {
        *out << range_miss_string;
    }


    static void print_enemy_trapped_string()
    {
        *out << enemy_trapped_string;
    }

    static void print_enemy_died_from_trap_string()
    {
        *out << enemy_died_from_trap_string;
    }

    static void print_trap_destroyed_string()
    {
        *out << trap_destroyed_string;
    }

    static void print_trap_placed_string()
    {
        *out << trap_placed_string;
    }

    static void print_getting_ready_string()
    {
        *out << getting_ready_string;
    }

    static void print_game_status(int playerHP, int playerXP, int enemyHP, int enemy2HP, int enemyTowerHP)
    {
        *out << "HP игрока: " << playerHP << std::endl;
        *out << "Накопленные XP: " << playerXP << std::endl;
        *out << "HP врага: " << enemyHP << std::endl;
        *out << "HP врага 2: " << enemy2HP << std::endl;
        *out << "HP вражеской башни: " << enemyTowerHP << std::endl;
        *out << "Новое заклинание стоит 5 XP, чтобы его получить, введите M." << std::endl
                  << "А что бы его использовать введите U." << std::endl;
    }

    static void print_take_damage(int dmg, std::string name)
    {
        *out << "Нанесено " << dmg << " урона " << name << "\n";
    }

    static void print_player_movement(int x, int y) {
        *out << "Игрок переместился на координаты (" << x << ", " << y << ")." << std::endl;
    }

    static void print_enemy_dead(std::string name)
    {
        *out << name << " погиб!\n";
    }

    static void print_entity_killed_other(std::string name, std::string other_name)
    {
        *out << name << " ликвидировал " << other_name << "!\n";
    }

    static void print_top_spell(int spells_size, int max_size)
    {
        *out << "--- Заклинания в руке (" << spells_size << "/" << max_size << ") ---" << std::endl;
    }

    static void print_spell_add(std::string spell_name)
    {
        *out << "В руку добавлено новое заклинание: " << spell_name << std::endl;
    }

    static void print_spell_cast(std::string spell_name)
    {
        *out << "Игрок применяет заклинание '" << spell_name << "'..." << std::endl;
    }

    static void print_spell_upgraded(std::string spell_name)
    {
        *out << "Заклинание '" << spell_name << "' было улучшено! " << std::endl;
    }

    static void print_spell_removed(int spellsToRemove)
    {
        *out << spellsToRemove << " заклинаний было утеряно при переходе." << std::endl;
    }

    static void print_upgrade_screen_header()
    {
        *out << "========================================" << std::endl;
        *out << "===       ВРЕМЯ УЛУЧШЕНИЙ!           ===" << std::endl;
        *out << "========================================" << std::endl;
        *out << "Выберите одну из наград за прохождение уровня:" << std::endl;
    }

    static void print_upgrade_options(int currentMaxHp, int currentHandSize)
    {
        *out << "\n1. Увеличить максимальное здоровье (Текущее: " << currentMaxHp << " -> " << currentMaxHp + 20 << ")" << std::endl;
        *out << "2. Увеличить размер руки (Текущий: " << currentHandSize << " -> " << currentHandSize + 1 << ")" << std::endl;
        *out << "3. Улучшить одно из заклинаний в руке" << std::endl;
        *out << "\nВаш выбор: ";
    }

    
    static void print_starting_new_game()
    {
        *out << starting_new_game_string;
    }

    static void print_game_saved()
    {
        *out << game_saved_string;
    }
    static void print_game_loaded()
    {
        *out << game_loaded_string;
    }

    static void print_menu_options()
    {
        *out << load_game_prompt_string;
        *out << start_game_prompt_string;
    }
    
    static void print_level_generation(int currentLevel)
    {
        *out << "Генерация уровня " << currentLevel << "..." << std::endl;
    }
    
    static void print_prompt_for_upgrade()
    {
        *out << prompt_for_upgrade_string;
    }

    static void print_level_completed(int currentLevel)
    {
        *out << "Поздравляем! Уровень " << currentLevel << " пройден!" << std::endl;
    }

    static void print_load_game_error(const std::exception &e)
    {
        
        
        *out << "[ERROR] Не удалось загрузить игру: " << e.what() << std::endl;
        if (out != &std::cout) out->flush();
    }
    
    static void print_save_error(const std::exception &e)
    {
        *out << "[ERROR] Ошибка сохранения: " << e.what() << std::endl;
        if (out != &std::cout) out->flush();
    }

    static void print_load_error(const std::exception &e)
    {
        *out << "[ERROR] Ошибка загрузки: " << e.what() << std::endl;
        if (out != &std::cout) out->flush();
    }
};

#endif