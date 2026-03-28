#ifndef GAME_H
#define GAME_H

#include "GameField.h"
#include "ISpell.h"
#include <string>
#include <fstream>
#include <stdexcept>
#include <memory>

void invalid_input();
void wait_moves();

class Game {
public:
    Game(): game_running_(false),
            current_level_(1),
            field_(nullptr) {}
    
    ~Game() = default;
    
    void start();
    void run();
    
    bool save_game();
    bool load_game(const std::string& filename);
    void show_save_files();
    
private:
    void initialize_current_level();
    void show_main_menu();
    void process_player_turn();
    bool check_win_condition();
    bool check_lose_condition();
    void level_complete();
    void game_over();
    
    std::string serialize() const;
    
    void deserialize_player(std::ifstream& file);
    void deserialize_field(std::ifstream& file);
    void deserialize_enemy(std::ifstream& file);
    void deserialize_tower(std::ifstream& file);
    void deserialize_ally(std::ifstream& file);

    ISpell* create_spell_from_name(const std::string& name, int level);

    int get_validated_input(const std::string& prompt, int min, int max);

    std::unique_ptr<GameField> field_;
    bool game_running_;
    int current_level_;
};

#endif