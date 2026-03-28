#include "game.h"
#include "save_load_manager.h"
#include "enemy.h"
#include "constants.h"
#include "event_system.h"
#include "logger.h"
#include <iostream>
#include <algorithm>
#include <random>

Game::Game() : current_level(1), game_running(false), total_turns(0) {}

Game::Game(Game&& other) noexcept 
    : field(std::move(other.field)),
      player(std::move(other.player)),
      current_level(other.current_level),
      game_running(other.game_running),
      total_turns(other.total_turns){
        if (player && field){
            player->set_field(field.get());
        }
}

Game& Game::operator=(Game&& other) noexcept{
    if (this != &other) {
        field = std::move(other.field);
        player = std::move(other.player);
        current_level = other.current_level;
        game_running = other.game_running;
        total_turns = other.total_turns;
    }
    return *this;
}

void Game::start(){
    game_running = true;
    game_loop();
}

void Game::start_new_game(){
    current_level = 1;
    total_turns = 0;
    player = std::make_unique<Player>("Маг", nullptr);
    init_level();
    start();
}

void Game::init_level(){
    int base_size = MIN_FIELD_SIZE;
    int max_increase = MAX_FIELD_SIZE - MIN_FIELD_SIZE;
    int level_increase = std::min(current_level - 1, max_increase / 2);
    int field_size = base_size + level_increase * 2;
    field_size = std::min(field_size, MAX_FIELD_SIZE);
    field_size = std::max(field_size, MIN_FIELD_SIZE);
    field = std::make_unique<Game_field>(field_size, field_size, player.get());
    //размещение игрока
    int start_x = std::max(1, field_size / 4);
    int start_y = std::max(1, field_size / 4);
    field->place_player(start_x, start_y);

    int enemy_count = 2 + current_level;
    enemy_count = std::min(enemy_count, 8);
    for (int i = 0; i < enemy_count; ++i){
        int attempts = 0;
        while (attempts < 10){
            int x = rand() % field_size;
            int y = rand() % field_size;
            auto player_pos = field->get_player_position();
            double dist = field->calculate_distance(x, y, player_pos.first, player_pos.second);
            if (dist > 3.0 && field->is_valid_position(x, y)){
                Cell cell = field->get_cell(x, y);
                if (cell.get_type() != Cell_type::BLOCKED){
                    std::string enemy_name = "Враг_L" + std::to_string(current_level);
                    field->add_enemy(new Enemy(enemy_name, field.get()), x, y);
                    break;
                }
            }
            attempts++;
        }
    }
    if (current_level >= 2){
        field->add_enemy_tower("Башня_Л" + std::to_string(current_level), 2, 2);
        if (field_size > 15){ 
            field->add_enemy_tower("Башня_Л" + std::to_string(current_level), field_size - 3, field_size - 3);
        }
    }
    if (current_level >= 3){
        field->add_building("Казарма_Л" + std::to_string(current_level), field_size -2, 2, SPAWN_INTERVAL);
    }
}

void Game::game_loop(){
    while (game_running){
        total_turns++;
        std::cout << "=== УРОВЕНЬ " << current_level << " ===\n";
        std::cout << "=== ХОД " << total_turns << " ===\n";
        field->display_field();
        player->display_status();
        player->restore_mana(MANA_RESTORE_PER_TURN);
        
        player_turn();
        if (!game_running) break;
        if (check_win_condition()){
            std::cout << "\n=== УРОВЕНЬ ПРОЙДЕН! ===\n";
            next_level();
            continue;
        }
        if (check_lose_condition()){
            game_over();
            break;
        }
        allies_turn();
        enemies_turn();
        buildings_turn();
        if (check_lose_condition()){ //проверка после хода врагов
            game_over();
            break;
        }
        std::cout << "\nНажмите Enter для продолжения...";
        std::cin.ignore();
        std::cin.get();
    }
}

void Game::player_turn(){
    std::cout << "\n--- Ваш ход ---\n";
    std::cout << "Команды: w/a/s/d - движение, e - атака, h - лечение, q - смена режима\n";
    std::cout << "Заклинания: 1-5 - применить, l - показать заклинания, n - выучить случайное\n";
    std::cout << "Система: save - сохранить, exit - выход\n";
    std::cout << "Введите команду: ";
    std::string input;
    std::cin >> input;

    if (input == "save"){
        std::cout << "Введите имя файла для сохранения: ";
        std::string file_name;
        std::cin >> file_name;
        save_game(file_name);
        return;
    }
    if (input == "exit"){
        game_running = false;
        return;
    }
    auto player_pos = field->get_player_position();
    if (input == "w") field->move_player(0, -1);
    else if (input == "s") field->move_player(0, 1);
    else if (input == "a") field->move_player(-1, 0);
    else if (input == "d") field->move_player(1, 0);
    else if (input == "e"){
        auto enemies = field->get_enemy_positions();
        if (!enemies.empty()){
            field->perform_attack(player_pos.first, player_pos.second, enemies[0].first, enemies[0].second);
        } else{
            std::cout << "Нет врагов для атаки!\n";
        }
    }
    else if (input == "h"){
        player->heal();
    }
    else if (input == "q"){
        player->switch_combat_mode();
    }
    else if (input == "l"){
        player->get_spell_hand().display_hand();
    }
    else if (input == "n"){
        player->learn_random_spell();
    }
    else if (input >= "1" && input <= "5"){
        int spell_index = std::stoi(input) - 1;
        auto& hand = player->get_spell_hand();
        if (spell_index >= 0 && static_cast<size_t>(spell_index) < hand.get_spell_count()){
            std::cout << "Введите координаты цели (x y): ";
            int target_x, target_y;
            std::cin >> target_x >> target_y;
            if (hand.cast_spell(spell_index, *player, target_x, target_y, *field)){
                std::cout << "Заклинание применено успешно!\n";
            } else{
                std::cout << "Не удалось применить заклинание!\n";
            }
        } else{
            std::cout << "В этом слоте нет заклинания!\n";
        }
    }
    else{
        std::cout << "Неизвестная команда!\n";
    }
}

void Game::allies_turn(){
    field->update_allies();
}

void Game::enemies_turn(){
    field->update_enemies();
}

void Game::buildings_turn(){
    field->update_buildings();
}

bool Game::check_win_condition() const{
    return field->get_enemy_count() == 0;
}

bool Game::check_lose_condition() const{
    return !player->is_alive();
}

void Game::remove_half_spells(){
    auto& hand = player->get_spell_hand();
    size_t spell_count = hand.get_spell_count();
    if (spell_count <= 1) return;
    size_t spells_to_remove = spell_count / 2;
    std::vector<size_t> indices(spell_count); 
    for (size_t i = 0; i < spell_count; ++i) indices[i] = i;
    std::random_device rd;
    //генератор случайных чисел
    std::mt19937 g(rd());
    std::shuffle(indices.begin(), indices.end(), g);
    for (size_t i = 0; i < spells_to_remove; ++i){
        hand.remove_spell(indices[i]);
    }
    std::cout << "Удалено " << spells_to_remove << " заклинаний при переходе на следующий уровень\n";
}

void Game::next_level(){
    current_level++;
    player->set_health(player->get_max_health());
    remove_half_spells();
    show_level_up_menu();
    init_level();
    std::cout << "Начинается уровень " << current_level << "!\n";
}

void Game::show_level_up_menu() {
    std::cout << "\n=== МЕНЮ ПРОКАЧКИ ===\n";
    std::cout << "Вы прошли уровень! Выберите улучшение:\n";
    std::cout << "1. +" << PLAYER_HEALTH_PER_LEVEL << " к максимальному здоровью\n";
    std::cout << "2. +" << PLAYER_MELEE_DAMAGE_PER_LEVEL << " к ближнему урону\n";
    std::cout << "3. +" << PLAYER_RANGED_DAMAGE_PER_LEVEL << " к дальнему урону\n";
    std::cout << "4. +10 к максимальной мане\n";
    std::cout << "5. Улучшить случайное заклинание\n";
    std::cout << "Выбор: ";

    int choice;
    std::cin >> choice;

    switch (choice){
        case 1:
            player->upgrade_max_health(PLAYER_HEALTH_PER_LEVEL);
            break;
        case 2:
            player->upgrade_melee_damage(PLAYER_MELEE_DAMAGE_PER_LEVEL);
            break;
        case 3:
            player->upgrade_ranged_damage(PLAYER_RANGED_DAMAGE_PER_LEVEL);
            break;
        case 4:
            player->upgrade_max_mana(10);
            break;
        case 5:
            player->enhance_random_spell();
            break;
        default:
            std::cout << "Неверный выбор, улучшение не применено.\n";
    }
}

void Game::game_over(){
    std::cout << "\n=== ИГРА ОКОНЧЕНА ===\n";
    std::cout << "Достигнут уровень: " << current_level << "\n";
    std::cout << "Всего ходов: " << total_turns << "\n";
    game_running = false;
    
    std::cout << "Хотите начать заново? (y/n): ";
    char choice;
    std::cin >> choice;
    
    if (choice == 'y' || choice == 'Y') {
        restart_with_new_system();
    }
}

bool Game::load_game(const std::string& file_name){
    try{
        *this = Save_load_manager::load_game(file_name);
        std::cout << "Игра загружена из файла: " << file_name << "\n";
        return true;
    } catch (const std::exception& e){
        std::cout << "Ошибка загрузки: " << e.what() << "\n";
        return false;
    }
}

bool Game::save_game(const std::string& file_name) const{
    try{
        Save_load_manager::save_game(*this, file_name);
        std::cout << "Игра сохранена в файл: " << file_name << "\n";
        return true;
    } catch (const std::exception& e){
        std::cout << "Ошибка сохранения: " << e.what() << "\n";
        return false;
    }
}

int Game::get_current_level() const{
    return current_level; 
}
int Game::get_total_turns() const{
    return total_turns; 
}
Player* Game::get_player() const{
    return player.get();
}
    
Game_field* Game::get_field() const{
    return field.get();
}
    
void Game::set_current_level(int level){
    current_level = level; 
}

void Game::set_total_turns(int turns){ 
    total_turns = turns; 
}

void Game::initialize_objects(int width, int height) {
    player = std::make_unique<Player>("Маг", nullptr);
    field = std::make_unique<Game_field>(width, height, player.get());
    player->set_field(field.get());
}

// для 4 лр
bool Game::is_running() const { 
    return game_running; 
}

void Game::update() {
    if (!game_running) return;
    
    if (check_win_condition()) {
        std::cout << "\n=== УРОВЕНЬ ПРОЙДЕН! ===\n";
        next_level();
    } else if (check_lose_condition()) {
        game_over();
    } else {
        allies_turn();
        enemies_turn();
        buildings_turn();
        
        if (check_lose_condition()) {
            game_over();
        }
    }
    
    total_turns++;
}

void Game::stop_game() { 
    game_running = false; 
}

void Game::handle_move(int dx, int dy) {
    if (field && player && game_running) {
        auto old_pos = field->get_player_position();
        field->move_player(dx, dy);
        auto new_pos = field->get_player_position();
        
        Event_system::get_instance().notify(Game_event(Event_type::PLAYER_MOVED, player->get_name(), "", 0, new_pos.first, new_pos.second, "Перемещение игрока"));
        external_update();
    }
}

void Game::handle_attack() {
    if (player && field && game_running) {
        auto player_pos = field->get_player_position();
        auto enemies = field->get_enemy_positions();
        if (!enemies.empty()) {
            field->perform_attack(player_pos.first, player_pos.second, enemies[0].first, enemies[0].second);
            
            Event_system::get_instance().notify(Game_event(Event_type::PLAYER_ATTACKED, player->get_name(), "враг", player->get_damage()));
        }
        external_update();
    }
}


void Game::handle_switch_mode() {
    if (player) {
        player->switch_combat_mode();
    }
}

void Game::handle_heal() {
    if (player) {
        player->heal();
    }
}

/*
void Game::handle_cast_spell(int spell_index, int target_x, int target_y) {
    if (player && field) {
        auto& hand = player->get_spell_hand();
        if (spell_index >= 0 && static_cast<size_t>(spell_index) < hand.get_spell_count()) {
            if (hand.cast_spell(spell_index, *player, target_x, target_y, *field)) {
                std::cout << "Заклинание применено успешно!\n";
                external_update();
            } else {
                std::cout << "Не удалось применить заклинание!\n";
            }
        } else {
            std::cout << "В этом слоте нет заклинания!\n";
        }
    }
}
*/

void Game::handle_cast_spell(int spell_index, int target_x, int target_y) {
    std::cout << "=== ОТЛАДКА ЗАКЛИНАНИЯ ===\n";
    std::cout << "Индекс: " << spell_index << ", Цель: (" << target_x << "," << target_y << ")\n";
    
    if (!player) {
        std::cout << "Ошибка: игрок не инициализирован!\n";
        return;
    }
    if (!field) {
        std::cout << "Ошибка: поле не инициализировано!\n";
        return;
    }
    
    auto& hand = player->get_spell_hand();
    std::cout << "Заклинаний в руке: " << hand.get_spell_count() << "\n";
    
    if (spell_index < 0 || static_cast<size_t>(spell_index) >= hand.get_spell_count()) {
        std::cout << "Ошибка: неверный индекс заклинания!\n";
        return;
    }
    
    const Spell_card* spell = hand.get_spell(spell_index);
    if (!spell) {
        std::cout << "Ошибка: заклинание не найдено!\n";
        return;
    }
    
    std::cout << "Заклинание: " << spell->get_name() 
              << ", Мана: " << player->get_mana() << "/" << spell->get_mana_cost() << "\n";
    
    if (hand.cast_spell(spell_index, *player, target_x, target_y, *field)) {
        std::cout << "Заклинание применено успешно!\n";
        Event_system::get_instance().notify(Game_event(Event_type::PLAYER_CAST_SPELL, player->get_name(), spell->get_name(), 0, target_x, target_y));
        external_update();
    } else {
        std::cout << "Не удалось применить заклинание!\n";
        std::cout << "Причина: недостаточно маны или неверная цель\n";
    }
    std::cout << "======================\n";
}

void Game::handle_show_spells() {
    if (player) {
        player->get_spell_hand().display_hand();
    }
}

void Game::handle_learn_spell() {
    if (player) {
        player->learn_random_spell();
    }
}
/*
void Game::handle_save(const std::string& filename) {
    save_game(filename);
}*/

void Game::handle_save(const std::string& filename) {
    std::cout << "DEBUG: handle_save called with filename: " << filename << std::endl;
    
    if (save_game(filename)) {
        Event_system::get_instance().notify(
            Game_event(Event_type::GAME_SAVED, "System", filename));
        
        std::cout << "Событие сохранения сгенерировано" << std::endl;
    } else {
        std::cout << "Сохранение не удалось, событие не генерируется" << std::endl;
    }
}

void Game::handle_load(const std::string& filename) {
    load_game(filename);
}

void Game::handle_quit() {
    game_running = false;
    std::cout << "Выход из игры..." << std::endl;
}

// для лр 4

void Game::start_external() {
    game_running = true;
    // Не вызываем game_loop() - цикл будет внешним
}

void Game::external_update() {
    if (!game_running) return;
    
    total_turns++;

    allies_turn();
    enemies_turn();
    buildings_turn();

    // Выполняем логику, которая обычно идет после хода игрока в game_loop()
    if (player) {
        player->restore_mana(MANA_RESTORE_PER_TURN);
    }
    
    if (check_win_condition()) {
        std::cout << "\n=== УРОВЕНЬ ПРОЙДЕН! ===\n";
        next_level();
    } else if (check_lose_condition()) {
        game_over();
    } else {
        allies_turn();
        enemies_turn();
        buildings_turn();
        
        if (check_lose_condition()) {
            game_over();
        }
    }
    
}

void Game::start_new_game_external() {
    current_level = 1;
    total_turns = 0;
    player = std::make_unique<Player>("Маг", nullptr);
    init_level(); // Это сохранит логику увеличения поля на следующих уровнях
    game_running = true;
    // НЕ вызываем game_loop() - цикл будет внешним
}

void Game::restart_with_new_system() {
    current_level = 1;
    total_turns = 0;
    player = std::make_unique<Player>("Маг", nullptr);
    init_level(); // Используем стандартную инициализацию уровня
    game_running = true;
    // НЕ вызываем game_loop() - цикл будет внешним
}

