#include "Game.h"
#include <cmath>
#include <cstdio>
#include <typeinfo>

Game::Game() : gen(std::random_device{}()), 
    level(1), 
    field(WIDTH, HEIGHT),
    enemies(NUMBER_OF_ENEMIES), 
    towers(NUMBER_OF_TOWERS)
    {this->counter_of_enemies = NUMBER_OF_ENEMIES;
    this->counter_of_towers = NUMBER_OF_TOWERS;
    this->counter_of_holes = NUMBER_OF_HOLES;}

Game::Game(int lvl) : gen(std::random_device{}()), 
    level(lvl), 
    field(WIDTH + (lvl / 2), HEIGHT + (lvl / 2)),
    enemies(NUMBER_OF_ENEMIES + (lvl - lvl / 3)),
    towers(NUMBER_OF_TOWERS + lvl / 3)
    {this->counter_of_enemies = NUMBER_OF_ENEMIES + (lvl - lvl / 3);
    this->counter_of_towers = NUMBER_OF_TOWERS + lvl / 3;
    this->counter_of_holes = NUMBER_OF_HOLES + lvl;}

void Game::place_object_at_random(ObjectType obj, std::uniform_int_distribution<int>& distrib_x, std::uniform_int_distribution<int>& distrib_y, Creature *creature){
    int x;
    int y;
    do {
        x = distrib_x(gen);
        y = distrib_y(gen);
    } while(!field.place_object_field(x, y, obj, creature));
    if(creature){
        creature->set_current_cell(field.get_cell(x, y));
    }
}

void Game::initialize() {
    if(this->level == 1){
        this->hand.add_spell(0);
    }
    else{
        std::ifstream ifs("buffer");
        if (!ifs.is_open()) {
            std::cerr << "ERROR: Не удалось загрузить руку\n";
            return;
        }
        
        try {
            hand.load(ifs);
        } catch (const std::exception& e) {
            std::cerr << "ERROR: Ошибка загрузки данных: " << e.what() << ". Файл поврежден.\n";
            ifs.close();
            return;
        }

        if (ifs.fail() && !ifs.eof()) {
            std::cerr << "ERROR: Файл сохранения содержит неполные или некорректные данные.\n";
            return;
        }

        ifs.close();
        hand.remove_spells();
    }
    std::uniform_int_distribution<int> distrib_x(0, this->field.getWidth() - 1);
    std::uniform_int_distribution<int> distrib_y(0, this->field.getHeight() - 1);

    int player_x = distrib_x(gen);
    int player_y = distrib_y(gen);
    this->player.set_current_cell(field.get_cell(player_x, player_y));
    field.place_object_field(player_x, player_y, ObjectType::Player, &this->player);

    for(int i = 0; i < this->counter_of_enemies; i++) {
        this->place_object_at_random(ObjectType::Enemy, distrib_x, distrib_y, &enemies[i]);
    }

    for(int i = 0; i < this->counter_of_towers; i++) {
        this->place_object_at_random(ObjectType::Tower, distrib_x, distrib_y, &towers[i]);
    }
    
    for(int i = 0; i < this->counter_of_holes; i++) {
        this->place_object_at_random(ObjectType::Hole, distrib_x, distrib_y, NULL);
    }
}

int Game::enemy_turn(){
    for(size_t i = 0; i < enemies.size(); i++){
        if(!enemies[i].is_dead){
            if((std::abs(this->player.get_current_cell()->getX() - this->enemies[i].get_current_cell()->getX()) + std::abs(this->player.get_current_cell()->getY() - this->enemies[i].get_current_cell()->getY())) == 1){
                this->player.deal_damage(enemies[i].getDamage());
                if(this->player.is_dead){
                    std::cout << "GAME OVER";
                    return 1;
                }
            }
            else{
                bool moved = false;
                if(this->player.get_current_cell()->getX() != this->enemies[i].get_current_cell()->getX()){
                    if(this->player.get_current_cell()->getX() > this->enemies[i].get_current_cell()->getX() && this->enemies[i].move(1, 0, &(this->field))){
                        moved = true;
                    }
                    else if(this->player.get_current_cell()->getX() < this->enemies[i].get_current_cell()->getX() && this->enemies[i].move(-1, 0, &(this->field))){
                        moved = true;
                    }
                }
                if(!moved){
                    if(this->player.get_current_cell()->getY() > this->enemies[i].get_current_cell()->getY()){
                        this->enemies[i].move(0, 1, &(this->field));
                    }
                    else if(this->player.get_current_cell()->getY() < this->enemies[i].get_current_cell()->getY()){
                        this->enemies[i].move(0, -1, &(this->field));
                    }
                }
                if(enemies[i].getIs_dead()){
                    counter_of_enemies--;
                    player.kill_up();
                    this->field.remove_object_field(enemies[i].get_current_cell()->getX(), enemies[i].get_current_cell()->getY());
                    if(this->counter_of_enemies == 0 && this->counter_of_towers == 0){
                        std::cout << "YOU WON!!!\n";
                        this->level++;
                        this->new_lvl = this->level;
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

int Game::tower_turn(){
    for(size_t i = 0; i < towers.size(); i++){
        if(!towers[i].is_dead){
            if((std::abs(this->player.get_current_cell()->getX() - this->towers[i].get_current_cell()->getX()) + std::abs(this->player.get_current_cell()->getY() - this->towers[i].get_current_cell()->getY())) <= this->towers[i].tower_range){
                this->towers[i].attack(&this->player);
                if(this->player.is_dead){
                    std::cout << "GAME OVER";
                    return 1;
                }
            }
            else{
                if(!this->towers[i].is_charged){
                    this->towers[i].attack(&this->player);
                }
            }
        }
    }
    return 0;
}

bool Game::save_game(const std::string& filename) {
    std::ofstream ofs(filename, std::ios::out | std::ios::trunc);
    if (!ofs.is_open()) {
        std::cerr << "ERROR: Невозможно открыть файл для записи: " << filename << "\n";
        return false;
    }

    try {
        ofs << enemies.size() << " " << towers.size() << " ";
        ofs << level << " ";
        ofs << counter_of_enemies << " " << counter_of_towers << " " << counter_of_holes << "\n";
        
        field.save(ofs);
        
        player.save(ofs);
        
        for (size_t i = 0; i < enemies.size(); i++) {
            enemies[i].save(ofs);
        }
        
        for (size_t i = 0; i < towers.size(); i++) {
            towers[i].save(ofs);
        }
        
        hand.save(ofs);
        
    } catch (...) {
        std::cerr << "ERROR: Ошибка записи данных в файл.\n";
        ofs.close();
        return false;
    }

    ofs.close();
    std::cout << "Игра успешно сохранена в " << filename << "\n";
    return true;
}

bool Game::load_game(const std::string& filename) {
    std::ifstream ifs(filename);
    if (!ifs.is_open()) {
        std::cerr << "ERROR: Файл сохранения не найден: " << filename << "\n";
        return false;
    }
    
    Field temp_field;
    
    try {
        int enemies_size;
        int towers_size;
        if (!(ifs >> enemies_size >> towers_size)) throw std::runtime_error("Некорректное количество врагов.");
        enemies.resize(enemies_size);
        towers.resize(towers_size);
        if (!(ifs >> level >> counter_of_enemies >> counter_of_towers >> counter_of_holes)) throw std::runtime_error("Некорректные счетчики.");
        
        field.load(ifs);
        temp_field = field;

        player.load(ifs);
        if (player.getLoadX() != -1) {
            int x = player.getLoadX();
            int y = player.getLoadY();
            player.set_current_cell(field.get_cell(x, y));
            field.place_object_field(x, y, ObjectType::Player, &player);
        }

        for (size_t i = 0; i < enemies.size(); i++) {
            enemies[i].load(ifs);
            if (!enemies[i].getIs_dead() && enemies[i].getLoadX() != -1) {
                int x = enemies[i].getLoadX();
                int y = enemies[i].getLoadY();
                enemies[i].set_current_cell(field.get_cell(x, y));
                field.place_object_field(x, y, ObjectType::Enemy, &enemies[i]);
            }
        }
        
        for (size_t i = 0; i < towers.size(); i++) {
            towers[i].load(ifs);
            if (!towers[i].getIs_dead() && towers[i].getLoadX() != -1) {
                int x = towers[i].getLoadX();
                int y = towers[i].getLoadY();
                towers[i].set_current_cell(field.get_cell(x, y));
                field.place_object_field(x, y, ObjectType::Tower, &towers[i]);
            }
        }
        
        hand.load(ifs);
        
    } catch (const std::exception& e) {
        std::cerr << "ERROR: Ошибка загрузки данных: " << e.what() << ". Файл поврежден.\n";
        ifs.close();
        return false;
    }

    if (ifs.fail() && !ifs.eof()) {
        std::cerr << "ERROR: Файл сохранения содержит неполные или некорректные данные.\n";
        return false;
    }

    ifs.close();
    std::cout << "Игра успешно загружена из " << filename << "\n";
    return true;
}