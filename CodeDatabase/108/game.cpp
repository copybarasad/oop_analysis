#include "game.h"
#include "direct_damage_spell.h"
#include "area_damage_spell.h"
#include "trap_spell.h"
#include <iostream>
#include <random>

#include "console_input_processor.h"
#include "console_renderer.h"
#include "game_controller.h"
#include "game_visualizer.h"



// 4 лб: (done)

// 1) система команд
// 2) шаблонный класс отрисовщика + параметр шаблона = консольный отрисовщик
// 3) шаблонный класс управления игрой + параметр шаблона = консольное считывание



GameLoop::GameLoop()
    : field(15, 10), player("Player", Position(0, 0)),
    current_turn(0), game_state(field), current_level(1), game_running(true), need_to_skip_enemies_turn(false){

    level_system.load_level(current_level, game_state, player);

    // Начальное заклинание
    player.get_spell_hand().add_spell(generate_random_spell());
}


ISpell* GameLoop::generate_random_spell() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 3);

    int spell_type = dis(gen);    // рандомно генерируется 1 из этих заклинаний раз в 8 ходов
    switch (spell_type) {
        case 0:
            return new DirectDamageSpell("Fireball", 20, 3);
        case 1:
            return new AreaDamageSpell("Firestorm", 15);
        case 2:
            return new TrapSpell("Bear Trap", 25);
        default:
            return new DirectDamageSpell("Ranged kick", 15, 4);
    }
}


// к 4 лб вынести в отдельный класс отрисовки + добавить выделение цветами -- DONE
// void GameLoop::render_field() const {
//
//     std::cout << "\n=== ХОД " << current_turn << " ===\n";
//     std::cout << "Игрок: Здоровье " << player.get_health()
//           << " | Позиция (" << player.get_position().get_x() << "," << player.get_position().get_y() << ")"
//           << " | Режим: " << (player.get_combat_mode() == CombatMode::MELEE ? "ближний бой" : "дальний бой")
//           << " | Урон: " << player.get_current_damage()
//           << " | Радиус: " << player.get_attack_range()
//           << " | Очки: " << player.get_score() << "\n\n";
//
//
//     // отрисовка поля
//     for (int y = field.get_height() - 1; y >= 0; --y) { // система координат как в математике (у - вверх), а рисуем вниз
//         std::cout << " ";
//         for (int x = 0; x < field.get_width(); ++x) {
//             Position pos(x, y);
//             char symbol = '.';
//
//             // Определяем символ для клетки
//             if (pos == player.get_position()) {
//                 symbol = 'P';
//             } else {
//
//                 bool has_entity = false;
//
//                 // Проверяем врагов
//                 for (const Enemy& enemy : game_state.get_enemies()) {
//                     if (enemy.is_alive() && enemy.get_position() == pos) {
//                         symbol = 'E';
//                         has_entity = true;
//                         break;
//                     }
//                 }
//
//                 // Проверяем спавнеры
//                 if (!has_entity) {
//                     for (const EnemySpawner& spawner : game_state.get_spawners()) {
//                         if (spawner.is_alive() && spawner.get_position() == pos) {
//                             symbol = 'S';
//                             has_entity = true;
//                             break;
//                         }
//                     }
//                 }
//
//                 // Проверяем башни
//                 if (!has_entity) {
//                     for (const EnemyTower& tower : game_state.get_towers()) {
//                         if (tower.is_alive() && tower.get_position() == pos) {
//                             symbol = 'T';
//                             has_entity = true;
//                             break;
//                         }
//                     }
//                 }
//
//                 // Если нет сущностей, показываем тип клетки
//                 if (!has_entity) {
//                     Cell cell = field.get_cell_at(pos);
//                     if (cell.get_type() == CellType::OBSTACLE) symbol = '#';
//                     else if (cell.get_type() == CellType::SLOW) symbol = '~';
//                     else if (cell.get_type() == CellType::TRAP) symbol = 'k';
//                 }
//             }
//
//             std::cout << symbol << " ";
//         }
//         std::cout << "\n";
//     }
//
//     std::cout << "\nУсловные обозначения: P-игрок, E-враг, S-спавнер, #-стена, ~-замедление, k-ловушка, T-Башня\n";
// }


void GameLoop::execute_cast_spell(int spell_index) {
    if (spell_index < player.get_spell_hand().get_spell_count()) {
        if (player.get_spell_hand().can_cast_spell(spell_index, player.get_position(), game_state)) {
            player.get_spell_hand().cast_spell(spell_index, player, game_state);    // и там в этом методе отладочный вывод тоже
        } else {
            std::cout << "Это заклинание сейчас недоступно!\n";
        }
    } else {
        std::cout << "Неверный номер заклинания!\n";
    }
}

void GameLoop::execute_kick() {
    if (player.attack_nearest_target(game_state)) {
        std::cout << "Атака успешна!\n";
    } else {
        std::cout << "Нет целей в радиусе атаки! ";
        std::cout << "Текущий режим: " << (player.get_combat_mode() == CombatMode::MELEE ? "ближний" : "дальний") << " бой\n";
        std::cout << "Радиус атаки: " << player.get_attack_range() << " клеток\n";
    }
}

void GameLoop::execute_load() {
    save_load_system.load_game(game_state, player, field, current_level, current_turn, level_system);
    need_to_skip_enemies_turn = true;
}

void GameLoop::execute_wasd(char symbol) {
    switch (symbol) {
        case 'w':
            player.move(Direction::UP, game_state);
            std::cout << "Движение вверх\n";
            break;
        case 'a':
            player.move(Direction::LEFT, game_state);
            std::cout << "Движение влево\n";
            break;
        case 's':
            player.move(Direction::DOWN, game_state);
            std::cout << "Движение вниз\n";
            break;
        case 'd':
            player.move(Direction::RIGHT, game_state);
            std::cout << "Движение вправо\n";
            break;
        default:
            std::cout << "Неверная команда! Пропускаем ход.\n";
    }
}

void GameLoop::execute_quit() {
    char choice;
    std::cout << "Вы уверены? (y/n)\n"; // чтобы не нажать по ошибке, перепутав с w/a
    std::cin >> choice;
    if (std::tolower(choice) == 'y') {
        std::cout << "Выход из игры...\n";
        exit(0);
    }
    std::cout << "Продолжение игры\n";
    need_to_skip_enemies_turn = true;
}

void GameLoop::execute_switch_combat_mode() {
    player.switch_combat_mode();
    std::cout << "Сменен режим боя\n";
}

void GameLoop::execute_save() {
    save_load_system.save_game(game_state, player, field, current_level, current_turn);
    need_to_skip_enemies_turn = true;
}


// void GameLoop::process_player_turn(int current) {  // вынесено
//
//     // к 4 лб вынести в отдельный класс считывания, пока для наглядности и отладки здесь
//     char choice;
//     std::cin >> choice;
//
//     switch (std::tolower(choice)) {
//         case 'w':
//             player.move(Direction::UP, game_state);
//             std::cout << "Движение вверх\n";
//             break;
//         case 'a':
//             player.move(Direction::LEFT, game_state);
//             std::cout << "Движение влево\n";
//             break;
//         case 's':
//             player.move(Direction::DOWN, game_state);
//             std::cout << "Движение вниз\n";
//             break;
//         case 'd':
//             player.move(Direction::RIGHT, game_state);
//             std::cout << "Движение вправо\n";
//             break;
//         case 'f':
//             player.switch_combat_mode();
//             std::cout << "Сменен режим боя\n";
//             break;
//         case 'e':
//             if (player.attack_nearest_target(game_state)) {
//                 std::cout << "Атака успешна!\n";
//             } else {
//                 std::cout << "Нет целей в радиусе атаки! ";
//                 std::cout << "Текущий режим: " << (player.get_combat_mode() == CombatMode::MELEE ? "ближний" : "дальний") << " бой\n";
//                 std::cout << "Радиус атаки: " << player.get_attack_range() << " клеток\n";
//             }
//             break;
//
//         // NEW BUTTONS
//         case 'l': // LOAD
//             save_load_system.load_game(game_state, player, field, current_level, current_turn, level_system);
//             need_to_skip_enemies_turn = true;
//             break;
//         case 'v': // SAVE
//             save_load_system.save_game(game_state, player, field, current_level, current_turn);
//             need_to_skip_enemies_turn = true;
//             break;
//
//         // обработка ввода заклинания
//         case '0':
//         case '1':
//         case '2':
//         case '3':
//         case '4': {
//             int spell_index = choice - '0';
//             if (spell_index < player.get_spell_hand().get_spell_count()) {
//                 if (player.get_spell_hand().can_cast_spell(spell_index, player.get_position(), game_state)) {
//                     player.get_spell_hand().cast_spell(spell_index, player, game_state);    // и там в этом методе отладочный вывод тоже
//                 } else {
//                     std::cout << "Это заклинание сейчас недоступно!\n";
//                 }
//             } else {
//                 std::cout << "Неверный номер заклинания!\n";
//             }
//             break;
//         }
//         case 'q':
//             std::cout << "Вы уверены? (y/n)\n"; // чтобы не нажать по ошибке, перепутав с w/a
//             std::cin >> choice;
//             if (std::tolower(choice) == 'y') {
//                 std::cout << "Выход из игры...\n";
//                 exit(0);
//             }
//             std::cout << "Продолжение игры\n";
//             need_to_skip_enemies_turn = true;
//             break;
//         default:
//             std::cout << "Неверная команда! Пропускаем ход.\n";
//     }
// }



void GameLoop::process_enemies_turn() {
    for (Enemy& enemy : game_state.get_enemies()) {

        Position enemy_pos = enemy.get_position();

        // проверяем, не встал ли он на ловушку
        if (field.get_cell_at(enemy_pos).get_type() == CellType::TRAP) {            // наступил => получает урон
            enemy.take_damage(field.get_cell_at(enemy_pos).get_trap_damage());
            std::cout << "Враг наступил на ловушку! Урон: " << field.get_cell_at(enemy_pos).get_trap_damage()
                      << " | Здоровье: " << enemy.get_health() << "\n";

            // remove trap
            field.set_or_remove_trap(enemy_pos, 0);

            if (!enemy.is_alive()) {
                std::cout << "Враг уничтожен ловушкой!\n";
                continue; // мертвый враг не двигается
            }
        }


        if (enemy.can_attack_player(player.get_position())) {
            player.take_damage(enemy.get_damage());
            std::cout << "Враг атаковал вас! Здоровье: " << player.get_health() << "\n";
        } else {
            enemy.move_towards_player(player.get_position(), game_state);    // враг перемещается, там учтен обход препятствий
        }
    }
}


void GameLoop::process_spawners_turn() {
    for (EnemySpawner& spawner : game_state.get_spawners()) {
        spawner.decrease_cooldown();
        if (spawner.should_spawn()) {
            Enemy new_enemy = spawner.spawn_enemy();
            game_state.add_enemy(new_enemy);
            std::cout << "Спавнер создал нового врага!\n";
        }
    }
}


void GameLoop::process_towers_turn() {
    for (EnemyTower& tower : game_state.get_towers()) { // там только живые, тк добавлено очищение убитых
        tower.decrease_cooldown();
        if (tower.can_attack_player(player)) {
            tower.attack_player(player, game_state);
        }
    }
}



void GameLoop::run() {
    std::cout << "=== ЗАПУСК ИГРЫ ===\n";
    std::cout << "Управление: WASD - движение, 0-4 - заклинания, E - удар, F - смена режима боя, L - загрузить, V - сохранить, Q - выход\n";
    std::cout << "Уровень " << current_level << "\n";

    GameVisualizer<ConsoleRenderer> visualizer(this);
    GameController<ConsoleInputProcessor> input_processor(this);


    while (game_running) {
        current_turn++;

        visualizer.render_game();


        // debug output
        if (current_turn % 8 == 0) {
            player.get_spell_hand().add_spell(generate_random_spell());
        }

        std::cout << "Выберите действие:\n";
        std::cout << "WASD - движение, 0-4 - заклинания, E - удар, F - смена режима боя, L - загрузить, V - сохранить, Q - выход\n";


        // Показываем доступные заклинания (для наглядности, чтобы пользователь понимал, что можно использовать, а что нет)
        for (int i = 0; i < player.get_spell_hand().get_spell_count(); ++i) {
            std::cout << i << " - " << player.get_spell_hand().get_spell_name(i);
            if (player.get_spell_hand().can_cast_spell(i, player.get_position(), game_state)) {
                std::cout << " ✅";
            } else {
                std::cout << " ❌";
            }
            std::cout << "\n";
        }
        std::cout << "Q - выйти из игры\n";



        //process_player_turn(current_turn); // было
        input_processor.process_command();   // стало



        if (need_to_skip_enemies_turn) {  // чтобы не тратить ход на сохранение и загрузку игры
            current_turn--;
            need_to_skip_enemies_turn = false; // сброс флага
            continue;
        }

        game_state.remove_dead_entities();

        if (check_level_completed()) {
            handle_level_completion();
            continue;
        }

        process_enemies_turn();
        process_spawners_turn();
        process_towers_turn();

        if (check_player_defeated()) {
            handle_game_over();
        }
    }
}


bool GameLoop::check_level_completed() const {
    return level_system.is_level_completed(game_state);
}

bool GameLoop::check_player_defeated() const {
    return !player.is_alive();
}


// handle methods:
void GameLoop::handle_level_completion() {
    std::cout << "\n=== УРОВЕНЬ " << current_level << " ПРОЙДЕН! ===\n";

    if (level_system.has_next_level()) {
        handle_level_transition();
    } else {
        handle_game_win();
    }
}

void GameLoop::handle_level_transition() {
    std::cout << "Переход на следующий уровень...\n";

    // Показываем меню улучшений, где выбирается улучшение
    show_upgrade_menu();

    // Переходим на следующий уровень
    current_level++;
    level_system.next_level(game_state, player);
    current_turn = 0;

    std::cout << "=== НАЧИНАЕТСЯ УРОВЕНЬ " << current_level << " ===\n";
}

void GameLoop::handle_game_over() {
    std::cout << "\n=== ИГРА ОКОНЧЕНА! ВЫ ПРОИГРАЛИ =( ===\n";
    std::cout << "Достигнут уровень: " << current_level << "\n";
    std::cout << "Набрано очков: " << player.get_score() << "\n";

    show_restart_menu();
}

void GameLoop::handle_game_win() {
    std::cout << "\n=== ПОБЕДА! ВСЕ УРОВНИ ПРОЙДЕНЫ! ===\n";
    std::cout << "Финальный счет: " << player.get_score() << "\n";

    show_restart_menu();
}


void GameLoop::show_upgrade_menu() {

    std::cout << "\n=== ВЫБОР УЛУЧШЕНИЯ ===\n";
    std::cout << "1. +30 к максимальному здоровью\n";
    std::cout << "2. +5 к ближнему урону\n";
    std::cout << "3. +5 к дальнему урону\n";
    std::cout << "4. +1 к радиусу атаки\n";
    std::cout << "5. +1 карточка заклинания в руку\n";
    std::cout << "Выберите улучшение: ";

    int choice;
    std::cin >> choice;

    switch(choice) {
        case 1:
            player.set_max_health(player.get_max_health() + 30);
            std::cout << "Здоровье увеличено до " << player.get_max_health() << "!\n";
            break;

        case 2:
            player.set_melee_damage(player.get_melee_damage() + 5);
            std::cout << "Ближний урон увеличен до " << player.get_melee_damage() << "!\n";
            break;

        case 3:
            player.set_ranged_damage(player.get_ranged_damage() + 5);
            std::cout << "Дальний урон увеличен до " << player.get_ranged_damage() << "!\n";
            break;

        case 4:
            player.set_melee_range(player.get_melee_range() + 1);
            player.set_ranged_range(player.get_ranged_range() + 1);

            std::cout << "Радиус атаки увеличен до " << player.get_melee_range() << " для ближнего боя и до " << player.get_ranged_range() << " для дальнего\n";
            break;

        case 5:
            player.get_spell_hand().add_spell(generate_random_spell());  // + debug output in this method
            if (player.get_spell_hand().get_spell_count() >= 5) {
                std::cout << "В руке нет места для еще одной карты\n";
            }
            break;
        default:
            std::cout << "Улучшение не выбрано.\n";
    }
}

void GameLoop::show_restart_menu() {
    std::cout << "\nХотите начать заново? (y/n): ";
    char choice;
    std::cin >> choice;

    if (std::tolower(choice) == 'y') {
        // Перезапускаем игру
        current_level = 1;
        current_turn = 0;
        player = Player("Player", Position(0, 0));
        level_system.load_level(current_level, game_state, player);
        player.get_spell_hand().add_spell(generate_random_spell());
    } else {
        game_running = false;
        std::cout << "\nЗавершение игры...\n";
    }
}


int GameLoop::get_current_turn() const {
    return current_turn;
}

const GameState &GameLoop::get_game_state() const {
    return game_state;
}

const Player &GameLoop::get_player() const {
    return player;
}


