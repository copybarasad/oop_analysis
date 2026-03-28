#include "game.h"
#include <iostream>
#include <limits>
#include <random>

const std::string Game::SAVE_FILE = "save_data.txt";

Game::Game()
:menu(Menu::Main_menu), current_level(1), player("PL"), place(15, 15), magic_tower(3), game_continue(true), game_controller(), render_controller(){}

void Game::run(){
    while(game_continue){
        try{
            switch(menu){
                case Menu::Main_menu:
                    main_menu();
                    break;

                case Menu::Level:
                    render_controller.render_game(*this, player);
                    player_turn();
                    
                    if (level_complete()){
                        menu = Menu::Level_complete;
                    } else if (game_over()){
                        menu = Menu::Game_over;
                    } else if (!game_continue) {
                        break;
                    } else {
                        enemy_turn();

                        if (level_complete()){
                            menu = Menu::Level_complete;
                        } else if (game_over()){
                            menu = Menu::Game_over;
                        }
                    }
                    break;
                
                case Menu::Level_complete:
                    render_controller.render_level_complete(*this);
                    menu = Menu::Upgrade_menu;
                    break;

                case Menu::Upgrade_menu:
                    upgrade_menu();
                    next_level();
                    menu = Menu::Level;
                    break;
                
                case Menu::Game_over:
                    render_controller.render_game_over(*this, player);

                    game_continue = false;
                    break;
            }
        } catch (const std::exception& e){
            std::cout << "Произошла ошибка: " << e.what() << "\n";
        }
    }
}

void Game::start_new_game(){
    delete_game();
    current_level = 1;
    create_level(current_level);
    menu = Menu::Level;
    std::cout << "Новая игра начата!\n";
}

bool Game::load_game(){
    try{
        std::ifstream file(SAVE_FILE, std::ios::binary);
        if (!file.is_open()){
            throw LoadException("Файл сохранения не найден");
        }

        Save_data data;
        file.read(reinterpret_cast<char*>(&data), sizeof(Save_data));

        if (file.fail()){
            throw LoadException("Ошибка чтения файла сохранения");
        }

        current_level = data.level;
        place = Place(data.base_x, data.base_y);
        
        player = Player("Pl", data.playerMaxHealth, data.playerHealth, data.playerPoints, 
                       data.playerDamage, data.playerX, data.playerY);
        std::vector<int> spell_types;
        for (int i = 0; i < data.spellCount && i < 10; i++) {
            spell_types.push_back(data.spellTypes[i]);
        }
        player.load_spells(spell_types);
        place.change_cell(Cell::Type::Player, data.playerX, data.playerY);
        
        enemies.clear();
        for (int i = 0; i < data.enemyCount; i++) {
            enemies.push_back(Enemy(data.enemyMaxHealth[i], data.enemyHealth[i], 
                                  data.enemyDamage[i], data.enemyX[i], data.enemyY[i], data.enemypoints[i]));
            place.change_cell(Cell::Type::Enemy, data.enemyX[i], data.enemyY[i]);
        }
        
        tower = std::make_unique<Tower>(5, enemies, data.towerMaxHealth, data.towerHealth, data.tower_x, data.tower_y);
        if (data.towerHealth > 0){
            place.change_cell(Cell::Type::Tower, data.tower_x, data.tower_y);
        }
        place.create_splecial_cell(Cell::Type::Obstacle);
        place.create_splecial_cell(Cell::Type::Slow);
        
        magic_tower = MagicTower(3, data.magicTowerMaxHealth, data.magicTowerHealth, data.magic_tower_x, data.magic_tower_y, data.towerspoints);
        if (!magic_tower.check_health()){
            place.change_cell(Cell::Type::Magic_tower, data.magic_tower_x, data.magic_tower_y);
        }

        movement = std::make_unique<Movement>(player, place);

        menu = Menu::Level;

        std::cout << "Игра загружена! Уровень: " << current_level << "\n";
        std::cout << "Размер поля: " << data.base_x << "x" << data.base_y << "\n";
        std::cout << "Врагов: " << data.enemyCount << "\n";
        file.close();
        return true;

    } catch (const LoadException& e) {
        std::cout << "Ошибка загрузки: " << e.what() << "\n";
        return false;
    } catch (const std::exception& e) {
        std::cout << "Неизвестная ошибка при загрузке: " << e.what() << "\n";
        return false;
    }
}
bool Game::save_game(){
    try{
        std::ofstream file(SAVE_FILE, std::ios::binary);
        if (!file.is_open()){
            throw SaveException("Не удалось создать файл сохранения");
        }

        Save_data data{};
        data.level = current_level;
        data.playerHealth = player.get_health();
        data.playerMaxHealth = player.get_maxHealth();
        data.playerDamage = player.get_damage();
        data.playerPoints = player.get_points();
        data.playerX = player.get_x();
        data.playerY = player.get_y();
        
        data.tower_x = tower->get_x();
        data.tower_y = tower->get_y();
        data.towerHealth = tower->get_health();
        data.towerMaxHealth = tower->get_maxHealth();
        data.towerspoints = tower->get_points();
        
        data.magic_tower_x = magic_tower.get_x();
        data.magic_tower_y = magic_tower.get_y();
        data.magicTowerHealth = magic_tower.get_health();
        data.magicTowerMaxHealth = magic_tower.get_maxHealth();
        
        data.enemyCount = enemies.size();
        for (size_t i = 0; i < enemies.size() && i < 20; i++) {
            data.enemyHealth[i] = enemies[i].get_health();
            data.enemyMaxHealth[i] = enemies[i].get_maxHealth();
            data.enemyDamage[i] = enemies[i].get_damage();
            data.enemyX[i] = enemies[i].get_x();
            data.enemyY[i] = enemies[i].get_y();
            data.enemypoints[i] = enemies[i].get_points();
        }
        data.base_x = place.get_rows();
        data.base_y = place.get_cols();
        std::vector<int> spell_types = player.get_spell_types();
        data.spellCount = spell_types.size();
        for (size_t i = 0; i < spell_types.size() && i < 3; i++) {
            data.spellTypes[i] = spell_types[i];
        }

        file.write(reinterpret_cast<const char*>(&data), sizeof(Save_data));

        if (file.fail()) {
            throw SaveException("Ошибка записи в файл сохранения");
        }

        std::cout << "Игра сохранена!\n";
        file.close();
        return true;
    } catch (const SaveException& e) {
        std::cout << "Ошибка сохранения: " << e.what() << "\n";
        return false;
    } catch (const std::exception& e) {
        std::cout << "Неизвестная ошибка при сохранении: " << e.what() << "\n";
        return false;
    }
    exit_game();
}
bool Game::exit_game(){
    game_continue = false;
    return true;
}

void Game::create_level(int level){

    srand(time(NULL));
    int base_x = rand() % (25 - 10 + 1) + 10;
    int base_y = rand() % (25 - 10 + 1) + 10;

    place = Place(base_x, base_y);

    enemies.clear();
    if (level == 1){
        player = Player("Pl");
    }
    place.create_splecial_cell(Cell::Type::Obstacle);
    place.create_splecial_cell(Cell::Type::Slow);
    place.change_cell(Cell::Type::Player, player.get_x(), player.get_y());

    int enemy_count = 2 + (level - 1);
    int enemy_health = 3 + (level - 1) * 2;
    int enemy_damage = 2 + (level - 1);
    int enemy_points = 1 * (3 / (level));


    for (int i = 0; i < enemy_count; i++){
        int x, y;
        do {
            x = rand() % (base_x - 1 + 1) + 1;
            y = rand() % (base_y - 1 + 1) + 1;
        } while (place.get_cell(x, y) != Cell::Type::Empty);

        enemies.push_back(Enemy(enemy_health, enemy_health, enemy_damage, x, y, enemy_points));
        place.change_cell(Cell::Type::Enemy, x, y);
    }
    int i = 3;
    int tower_x, tower_y;
    do {
        tower_x = base_x - i;
        tower_y = base_y - i;
        i++;
    } while (place.get_cell(tower_x, tower_y) != Cell::Type::Empty);
    
    int towerHealth = 3 + (level - 1) * 5;
    int tower_points = 3 * (3 / (level));

    tower = std::make_unique<Tower>(25, enemies, towerHealth, towerHealth, tower_x, tower_y, tower_points);
    place.change_cell(Cell::Type::Tower, tower_x, tower_y);

    int j = 5;
    int mag_tower_x, mag_tower_y;
    do {
        mag_tower_x = j;
        mag_tower_y = j;
        j++;
    } while (place.get_cell(mag_tower_x, mag_tower_y) != Cell::Type::Empty);
    int mag_towerHealth = 50 + (level - 1) * 5;
    int mag_tower_points = 3 + (3 / (level));

    magic_tower = MagicTower(3, mag_towerHealth, mag_towerHealth, mag_tower_x, mag_tower_y, mag_tower_points);
    place.change_cell(Cell::Type::Magic_tower, mag_tower_x, mag_tower_y);

    movement = std::make_unique<Movement>(player, place);
    std::cout << "Уровень " << level << " загружен! Размер поля: " << base_x << "x" << base_y << "\n";
}


void Game::next_level(){
    current_level++;
    player.heal();
    player.to_start();
    create_level(current_level);
}
bool Game::level_complete(){
    return tower->check_health() && enemies.empty();
}
bool Game::game_over(){
    return player.check_health();
}

void Game::player_turn(){
    int k = 0;
    int next = 0;
    while (k % 2 == 0){
        render_controller.render_player_turn(place, player, enemies, *tower, magic_tower);
        
        next += game_controller.level_command(*this);
        if (next == 2){
            k++; 
        } else if (next == 3) {
            render_controller.render_level(place, player, enemies, *tower, magic_tower);
            k++;
        } else if (next >= 6) {
            for (auto it = enemies.begin(); it != enemies.end(); ) {
                if (it->check_health()) {
                    place.change_cell(Cell::Type::Empty, it->get_x(), it->get_y());
                    it = enemies.erase(it);
                } else {
                    ++it;
                }
            }
            k++;
        } else if (next == 99){
            return;
        }
    } 
}

int Game::move(Movement::Direction dir){
    int res = movement->move(dir);
    return res;
}

int Game::attack(){
    std::string tar_ch;
    char target_choice;
    int res = 0;
    if (enemies.empty() && tower->check_health()) {
        std::cout << "\t\t\tНет целей для атаки\n";
        return res;
    }
    
    render_controller.render_choosing_target(enemies, *tower);
        
    std::cin >> tar_ch;
    target_choice = tar_ch[0];
        
    if (target_choice == 't') {
        res = player.attack(*tower) + 1;
    } else if (target_choice >= '0' && target_choice <= '9') {
        int enemy_index = target_choice - '0';
        if (enemy_index >= 0 && enemy_index < static_cast<int>(enemies.size())) {
            res = player.attack(enemies[enemy_index]) + 1;
        } else {
            std::cout << "\t\t\tНекорректный выбор врага\n";
        }
    } else {
        std::cout << "\t\t\tНекорректный выбор врага\n";
    }
    return res;
}

int Game::use_spell(){
    int x_co;
    int y_co;
    int sp;
    int res = 0;
    render_controller.render_using_spell(player);
    while (!(std::cin >> sp)){
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Введены некорректные данные" << std::endl;
        std::cout << "Введите номер заклинания: ";
    }

    std::cout << "Введите коордитнаты точки, в которой будет применено заклинание:\nx - ";
    while (!(std::cin >> y_co)){
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Введены некорректные данные" << std::endl;
        std::cout << "x - ";
    }
    std::cout << "y - ";
    while (!(std::cin >> x_co)){
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Введены некорректные данные" << std::endl;
        std::cout << "y - ";
    }
    res = player.use_spell(sp, x_co, y_co, place, enemies, *tower, magic_tower) + 1;
    return res;
}

int Game::buy_spell(){
    int res = player.buy_spell() + 1; 
    return res;
}
int Game::change_weapon(){
    int res = player.change_weapon() + 1; 
    return res;
}

void Game::enemy_turn(){
    render_controller.render_enemy_turn(place, player, enemies, *tower, magic_tower);
    
    tower->update(place);
    magic_tower.update(place, player, enemies, *tower);
    
    for (auto& enemy : enemies) {
        if (!enemy.check_health()) {
            enemy.action(player, place);
        }
    }
    for (auto it = enemies.begin(); it != enemies.end(); ) {
        if (it->check_health()) {
            std::cout << "\t\t\tВраг побеждён.\n\t\t\tУ вас " << player.get_points() << " очков." << '\n' << '\n';
            place.change_cell(Cell::Type::Empty, it->get_x(), it->get_y());
            it = enemies.erase(it);
        } else {
            ++it;
        }
    }

    if (tower->check_health()) {
        std::cout << "\t\t\tБашня уничтожена\n";
        place.change_cell(Cell::Type::Empty, tower->get_x(), tower->get_y());
    }
    if (magic_tower.check_health()) {
        std::cout << "\t\t\tБашня заклинаний уничтожена\n";
        place.change_cell(Cell::Type::Empty, magic_tower.get_x(), magic_tower.get_y());
    }
    }

void Game::upgrade_menu(){
    int n = player.remove_random_spell();
    if (n){
        std::cout << "\nПосле завершения уровня одно из ваших заклинаний было утеряно!\nTekuchie zaklinaniya\n";
    }
    render_controller.render_upgrade_menu(player);
    while (player.get_points() > 0) {
        render_controller.render_upgrade_options(player);
        game_controller.upgarde_command(*this);
    }
}

void Game::upgrade(char choice){
    if (choice == '0') return;
    
    switch (choice) {
        case '1':
            if (player.get_points() >= 2) {
                player.increase_max_health(5);
                player.increase_points(-2);
                std::cout << "Максимальное здоровье увеличено до " << player.get_maxHealth() << "\n";
            } else {
                std::cout << "Недостаточно очков\n";
            }
            break;
            
        case '2':
            if (player.get_points() >= 2) {
                player.increase_damage(2);
                player.increase_points(-2);
                std::cout << "Урон увеличен до " << player.get_damage() << "\n";
            } else {
                std::cout << "Недостаточно очков\n";
            }
            break;
            
        case '3':
            if (player.get_points() >= 3) {
                
                if (player.upgrade_spell(1)){
                    player.increase_points(-3);
                }
            } else {
                std::cout << "Недостаточно очков\n";
            }
            break;
            
        default:
            std::cout << "Неверный выбор\n";
            break;
    }
}
    
void Game::delete_game(){
    enemies.clear();
    tower.reset();
    movement.reset();
}


void Game::main_menu(){
    render_controller.render_main_menu();
    game_controller.main_menu_command(*this);
}

void Game::main_choice(char choice){
    switch (choice) {
        case '1':
            start_new_game();
            break;
        case '2':
            if (load_game()) {
                menu = Menu::Level;
            }
            break;
        case '3':
            game_continue = false;
            break;
        default:
            std::cout << "Неверный выбор\n";
            break;
    }
}

int Game::get_current_level(){
    return current_level;
}