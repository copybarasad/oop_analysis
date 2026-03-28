#include <iostream>
#include <cassert>
#include "player.h"
#include "enemy.h"
#include "game_field.h"
#include "cell.h"
#include "building.h"

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
    
    std::cout << "ПРОЙДЕН\n";
}

void test_player() {
    std::cout << "Тест Player... ";
    
    Player player("ТестовыйИгрок", nullptr);
    
    assert(player.get_name() == "ТестовыйИгрок");
    assert(player.get_health() == 100);
    assert(player.get_level() == 1);
    assert(player.get_is_alive());
    
    player.take_damage(30);
    assert(player.get_health() == 70);
    
    player.heal(20);
    assert(player.get_health() == 90);
    
    player.take_damage(100);
    assert(player.get_health() == 0);
    
    std::cout << "ПРОЙДЕН\n";
}

void test_enemy() {
    std::cout << "Тест Enemy... ";
    
    Enemy enemy("ТестовыйВраг", nullptr, 50, 10);
    
    assert(enemy.get_name() == "ТестовыйВраг");
    assert(enemy.get_health() == 50);
    assert(enemy.get_damage() == 10);
    assert(enemy.get_is_alive());
    
    enemy.take_damage(20);
    assert(enemy.get_health() == 30);
    assert(enemy.get_is_alive());
    
    enemy.take_damage(40);
    assert(enemy.get_health() == 0);
    assert(!enemy.get_is_alive());
    
    std::cout << "ПРОЙДЕН\n";
}

void test_game_field_basic() {
    std::cout << "Тест GameField (базовый)... ";
    
    Player player("Тестер", nullptr);
    Game_field field(10, 10, &player);
    
    assert(field.place_player(0, 0));
    auto pos = field.get_player_position();
    assert(pos.first == 0 && pos.second == 0);
    
    assert(field.move_player(1, 0) == Move_status::SUCCESS);
    pos = field.get_player_position();
    assert(pos.first == 1 && pos.second == 0);
    
    field.add_enemy(new Enemy("ТестВраг", &field), 5, 5);
    assert(field.get_enemy_positions().size() == 1);
    
    std::cout << "ПРОЙДЕН\n";
}

void test_game_field_combat() {
    std::cout << "Тест GameField (комбат)... ";
    
    Player player("Боец", nullptr);
    Game_field field(10, 10, &player);
    
    field.place_player(0, 0);
    Enemy* enemy = new Enemy("Мишень", &field, 100, 5);
    field.add_enemy(enemy, 1, 0); 
    
    int initial_health = enemy->get_health();
    field.perform_attack(0, 0, 1, 0);
    assert(enemy->get_health() < initial_health);
    
    std::cout << "ПРОЙДЕН\n";
}

void test_building() {
    std::cout << "Тест Building... ";
    
    Building building("ТестЗдание", 5, 5, nullptr, 3);
    
    assert(building.get_name() == "ТестЗдание");
    assert(building.get_x() == 5);
    assert(building.get_y() == 5);
    
    building.update();
    building.update();
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
    
    Game_field copy = original;
    assert(copy.get_width() == 12);
    assert(copy.get_height() == 12);
    
    Game_field assigned(8, 8, &player);
    assigned = original;
    assert(assigned.get_width() == 12);
    
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
    
    std::cout << "\n=== ВСЕ ТЕСТЫ ПРОЙДЕНЫ УСПЕШНО ===\n";
}

int main() {
    run_all_tests();
    return 0;
}