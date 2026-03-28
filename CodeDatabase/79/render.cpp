#include "render.h"
#include <iostream>

#include "place.h"
#include "enemy.h"
#include "tower.h"
#include "magic_tower.h"
#include "game.h"

void Render::render_main_menu(){
    std::cout << "\n=== ГЛАВНОЕ МЕНЮ ===\n";
    std::cout << "1 - Новая игра\n";
    std::cout << "2 - Загрузить игру\n";
    std::cout << "3 - Выйти\n";
}

void Render::render_level(Place& place, Player& player, std::vector<Enemy>& enemies, Tower& tower, MagicTower& magic_tower){
    render_place(place);
    render_player_stats(player);
    render_enemy_stats(enemies, tower, magic_tower);
}

void Render::render_player_turn(Place& place, Player& player, std::vector<Enemy>& enemies, Tower& tower, MagicTower& magic_tower){
    std::cout << "============================================= ХОД ИГРОКА =============================================" << std::endl;
    render_place(place);
    render_player_stats(player);
    render_enemy_stats(enemies, tower, magic_tower);
    std::cout << "Выберите действие:\tq - атаковать, работает только если ты рядом с врагом.\tc - сменить оружие.\tf - использовать магию\tg - купить заклинание\nИдти в определённом направлении:\tw - Вверх\ta - Влево\ts - Вниз\td - Вправо\n";
    std::cout << "p - Сохранить игру.\n";
    std::cout << "m - Выйти из игры.\n";
}

void Render::render_enemy_turn(Place& place, Player& player, std::vector<Enemy>& enemies, Tower& tower, MagicTower& magic_tower){
    std::cout << "============================================= ХОД ВРАГОВ =============================================" << std::endl;
    render_place(place);
    render_player_stats(player);
    render_enemy_stats(enemies, tower, magic_tower);
}
void Render::render_upgrade_menu(Player& player){
    player.get_spell_info();
    std::cout << "\n=== МЕНЮ УЛУЧШЕНИЙ ===\n";
    std::cout << "У вас есть очки улучшения: " << player.get_points() << "\n";
    std::cout << "Текущие характеристики:\n";
    player.print_current_stats();
}
void Render::render_upgrade_options(Player& player){
    std::cout << "\nОсталось очков улучшения: " << player.get_points() << "\n";
    std::cout << "Выберите улучшение:\n";
    std::cout << "1 - Увеличить максимальное здоровье (+5 HP, 2 очко)\n";
    std::cout << "2 - Увеличить урон (+2 урона, 2 очко)\n";
    std::cout << "3 - Улучшить заклинание (+1 к дальности атаки/ +1 k uronu, 3 очка)\n";
    std::cout << "0 - Закончить улучшения\n";
}

void Render::render_game_over(Game& game, Player& player){
    std::cout << "\n=== ИГРА ОКОНЧЕНА ===\n";
    std::cout << "Ваш результат: " << player.get_points() << " очков" << std::endl;
    std::cout << "Пройдено уровней: " << (game.get_current_level() - 1) << "\n\n";
}

void Render::render_level_complete(Game& game){
    std::cout << "\n=== УРОВЕНЬ " << game.get_current_level() << " ПРОЙДЕН! ===\n";
    std::cout << "Вы получаете возможность улучшить характеристики!\n";
}

void Render::render_game(Game& game, Player& player){
    std::cout << "\n=== УРОВЕНЬ " << game.get_current_level() << " ===\n";
    std::cout << "Очки: " << player.get_points() << "\n\n";
}

void Render::render_using_spell(Player& player){
    std::cout << "Доступные заклининия" << '\n';
    player.getInfo();
    std::cout << "Введите номер нужного заклинания: ";
}

void Render::render_choosing_target(std::vector<Enemy>& enemies, Tower& tower){
    std::cout << "Выберите цель:\n";
    std::cout << "t - башня (" << tower.get_health() << "/" << tower.get_maxHealth() << " HP)\n";
        
    for (size_t j = 0; j < enemies.size(); j++) {
        std::cout << j << " - враг " << j + 1 << " (" << enemies[j].get_health() << "/" << enemies[j].get_maxHealth() << " HP) координаты (" 
                    << enemies[j].get_x() << "," << enemies[j].get_y() << ")\n";
    }
}

void Render::render_place(Place& place){
    place.render();
}

void Render::render_player_stats(Player& player){
    player.print_current_stats();
}


void Render::render_enemy_stats(std::vector<Enemy>& enemies, Tower& tower, MagicTower& magic_tower){
    std::cout << "------ENEMIES------" << '\n';
    for (size_t j = 0; j < enemies.size(); j++) {
        std::cout << "Враг " << j + 1 << ": ";
            enemies[j].print_current_stats();
    }
            
    tower.print_current_stats();
    magic_tower.print_current_stats();
}