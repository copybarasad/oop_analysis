#include <iostream>
#include <cassert>
#include "player.h"
#include "enemy.h"
#include "game_field.h"
#include "cell.h"
#include "building.h"
#include "constants.h"

void test_cell() {
    std::cout << "Тест Cell... ";
    
    Cell empty_cell;
    assert(empty_cell.get_type() == Cell_type::EMPTY);
    assert(empty_cell.is_empty());
    assert(empty_cell.can_move_to());
    
    Cell blocked_cell(Cell_type::BLOCKED);
    assert(blocked_cell.get_type() == Cell_type::BLOCKED);
    assert(!blocked_cell.can_move_to());
    
    Cell slow_cell(Cell_type::SLOW);
    assert(slow_cell.get_type() == Cell_type::SLOW);
    assert(slow_cell.can_move_to());
    
    // Тест установки состояний
    empty_cell.set_has_player(true);
    assert(empty_cell.get_has_player());
    assert(!empty_cell.is_empty());
    assert(!empty_cell.can_move_to());
    
    empty_cell.set_has_player(false);
    empty_cell.set_has_enemy(true);
    assert(empty_cell.get_has_enemy());
    assert(!empty_cell.is_empty());
    
    empty_cell.set_has_enemy(false);
    empty_cell.set_has_building(true);
    assert(empty_cell.get_has_building());
    assert(!empty_cell.is_passable());
    
    std::cout << "ПРОЙДЕН\n";
}

void test_player() {
    std::cout << "Тест Player... ";
    
    Player player("ТестовыйИгрок", nullptr);
    
    assert(player.get_name() == "ТестовыйИгрок");
    assert(player.get_health() == DEFAULT_PLAYER_HEALTH);
    assert(player.get_max_health() == DEFAULT_PLAYER_HEALTH);
    assert(player.get_level() == 1);
    assert(player.is_alive());
    assert(player.get_melee_damage() == DEFAULT_PLAYER_MELEE_DAMAGE);
    assert(player.get_ranged_damage() == DEFAULT_PLAYER_RANGED_DAMAGE);
    
    // Тест получения урона
    player.take_damage(30);
    assert(player.get_health() == 70);
    assert(player.is_alive());
    
    // Тест лечения
    player.heal();
    assert(player.get_health() == 100); // HEAL_AMOUNT = 30, но ограничено максимумом
    
    // Тест смерти
    player.take_damage(150);
    assert(player.get_health() == 0);
    assert(!player.is_alive());
    
    // Тест повышения уровня
    Player level_player("Уровневый", nullptr);
    level_player.level_up();
    assert(level_player.get_level() == 2);
    assert(level_player.get_max_health() == DEFAULT_PLAYER_HEALTH + PLAYER_HEALTH_PER_LEVEL);
    assert(level_player.get_melee_damage() == DEFAULT_PLAYER_MELEE_DAMAGE + PLAYER_MELEE_DAMAGE_PER_LEVEL);
    
    // Тест смены режима боя
    assert(level_player.get_combat_mode() == Combat_mode::MELEE);
    level_player.switch_combat_mode();
    assert(level_player.get_combat_mode() == Combat_mode::RANGED);
    
    std::cout << "ПРОЙДЕН\n";
}

void test_enemy() {
    std::cout << "Тест Enemy... ";
    
    Enemy enemy("ТестовыйВраг", nullptr);
    
    assert(enemy.get_name() == "ТестовыйВраг");
    assert(enemy.get_health() == DEFAULT_ENEMY_HEALTH);
    assert(enemy.get_damage() == DEFAULT_ENEMY_DAMAGE);
    assert(enemy.get_is_alive());
    
    // Тест получения урона
    enemy.take_damage(20);
    assert(enemy.get_health() == 30);
    assert(enemy.get_is_alive());
    
    // Тест смерти
    enemy.take_damage(40);
    assert(enemy.get_health() == 0);
    assert(!enemy.get_is_alive());
    
    // Тест установки здоровья
    Enemy enemy2("Враг2", nullptr);
    enemy2.set_health(75);
    assert(enemy2.get_health() == 75);
    
    std::cout << "ПРОЙДЕН\n";
}

void test_game_field_basic() {
    std::cout << "Тест GameField (базовый)... ";
    
    Player player("Тестер", nullptr);
    Game_field field(10, 10, &player);
    
    assert(field.get_width() == 10);
    assert(field.get_height() == 10);
    
    // Тест размещения игрока
    assert(field.place_player(0, 0));
    auto pos = field.get_player_position();
    assert(pos.first == 0 && pos.second == 0);
    
    // Тест движения игрока
    assert(field.move_player(1, 0) == Move_status::SUCCESS);
    pos = field.get_player_position();
    assert(pos.first == 1 && pos.second == 0);
    
    // Тест добавления врага
    field.add_enemy(new Enemy("ТестВраг", &field), 5, 5);
    assert(field.get_enemy_positions().size() == 1);
    
    // Тест добавления здания
    field.add_building("ТестКазарма", 3, 3, 2);
    
    // Тест валидности позиций
    assert(field.is_valid_position(0, 0));
    assert(field.is_valid_position(9, 9));
    assert(!field.is_valid_position(-1, 0));
    assert(!field.is_valid_position(10, 10));
    
    std::cout << "ПРОЙДЕН\n";
}

void test_game_field_combat() {
    std::cout << "Тест GameField (комбат)... ";
    
    Player player("Боец", nullptr);
    Game_field field(10, 10, &player);
    
    field.place_player(0, 0);
    Enemy* enemy = new Enemy("Мишень", &field);
    field.add_enemy(enemy, 1, 0); 
    
    int initial_health = enemy->get_health();
    field.perform_attack(0, 0, 1, 0);
    assert(enemy->get_health() < initial_health);
    
    // Тест расчета расстояния
    assert(field.can_attack(0, 0, 1, 0, Combat_mode::MELEE));
    assert(!field.can_attack(0, 0, 3, 0, Combat_mode::MELEE));
    assert(field.can_attack(0, 0, 3, 0, Combat_mode::RANGED));
    
    std::cout << "ПРОЙДЕН\n";
}

void test_building() {
    std::cout << "Тест Building... ";
    
    Building building("ТестЗдание", 5, 5, nullptr, 3);
    
    assert(building.get_name() == "ТестЗдание");
    assert(building.get_x() == 5);
    assert(building.get_y() == 5);
    
    // Тест обновления и спавна
    building.update();
    building.update();
    assert(!building.should_spawn_enemy());
    
    building.update(); 
    assert(building.should_spawn_enemy());
    
    std::cout << "ПРОЙДЕН\n";
}

void test_field_copy_move() {
    std::cout << "Тест копирования/перемещения поля... ";
    
    Player player("Оригинал", nullptr);
    Game_field original(12, 12, &player);
    original.place_player(2, 2);
    original.add_enemy(new Enemy("Враг1", &original), 5, 5);
    
    // Тест конструктора копирования
    Game_field copy = original;
    assert(copy.get_width() == 12);
    assert(copy.get_height() == 12);
    
    // Тест оператора присваивания
    Game_field assigned(8, 8, &player);
    assigned = original;
    assert(assigned.get_width() == 12);
    
    // Тест конструктора перемещения
    Game_field moved(std::move(original));
    assert(moved.get_width() == 12);
    
    std::cout << "ПРОЙДЕН\n";
}

void test_constants() {
    std::cout << "Тест констант... ";
    
    assert(DEFAULT_PLAYER_HEALTH == 100);
    assert(DEFAULT_PLAYER_MELEE_DAMAGE == 15);
    assert(HEAL_AMOUNT == 30);
    assert(SCORE_PER_KILL == 10);
    assert(MIN_FIELD_SIZE == 10);
    assert(MAX_FIELD_SIZE == 25);
    
    std::cout << "ПРОЙДЕН\n";
}

void run_all_tests() {
    std::cout << "=== ЗАПУСК ТЕСТОВ ===\n\n";
    
    test_cell();
    test_player();
    test_enemy();
    test_game_field_basic();
    test_game_field_combat();
    test_building();
    test_field_copy_move();
    test_constants();
    
    std::cout << "\n=== ВСЕ ТЕСТЫ ПРОЙДЕНЫ УСПЕШНО ===\n";
}

int main() {
    try {
        run_all_tests();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Ошибка в тестах: " << e.what() << std::endl;
        return 1;
    }
}