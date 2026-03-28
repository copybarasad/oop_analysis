#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include "command.hpp"
#include "position.hpp"
#include "game_controller.hpp"
#include "save_and_load.hpp"
#include <iostream>
#include <filesystem>
#include <string>
#include <sstream>

class Move_command: public Command{
    private:
        Position offset;
    public:
        explicit Move_command(Position offset_): offset(offset_){}
        void execute(Game_controller& game) override{
            Position cur = game.get_player().get_position();
            Position new_pos(cur.get_x() + offset.get_x(), cur.get_y() + offset.get_y());
            game.process_player_move(new_pos);
    }
};

class Switch_combat_style_command: public Command{
    private:
        Combat_style style;
    public:
        explicit Switch_combat_style_command(Combat_style style_): style(style_){}
        void execute(Game_controller& game) override{
            game.process_player_combat_style_switch(style);
        }
};

class Ranged_attack_command: public Command{
    public:
        void execute(Game_controller& game) override{
            game.process_player_ranged_attack();
        }
};

class Spell_cast_command: public Command{
    private:
        int spell_index;
        Position target_pos;
    public:
        Spell_cast_command(int idx, Position pos): spell_index(idx), target_pos(pos){}
        void execute(Game_controller& game) override{
            game.process_player_spell_cast(spell_index, target_pos);
        }
};

class Buy_spell_command: public Command{
    public:
        void execute(Game_controller& game) override{
            game.process_player_buy_spell();
        }
};

class Save_game_command: public Command{
    private:
        std::string save_name;
    public:
        explicit Save_game_command(std::string name): save_name(std::move(name)){}
        void execute(Game_controller& game) override{
            std::string filename = save_name + ".txt";
            try{
                Save_and_load saver;
                saver.save_game(game, filename);
                std::cout << "Game saved successfully!\n";
            }
            catch(const std::exception& e){
                std::cout << "Save failed: " << e.what() << "\n";
            }
        }
};

class Load_game_command: public Command{
    public:
        void execute(Game_controller& game) override{
            namespace fs = std::filesystem;
            std::vector<std::string> saves;
            if(fs::exists("saves") && fs::is_directory("saves")){
                for(auto& entry: fs::directory_iterator("saves"))
                    if(entry.path().extension() == ".txt") saves.push_back(entry.path().filename().string());
            }
            if(saves.empty()){
                std::cout << "No saves found.\n";
                return;
            }
            std::cout << "Load which save?\n";
            for(size_t i = 0; i < saves.size(); i++){
                std::cout << (i + 1) << ") " << saves[i] << "\n";
            }
            std::cout << "Enter number: ";
            std::string choice;
            std::getline(std::cin, choice);
            try{
                int idx = std::stoi(choice) - 1;
                if(idx >= 0 && idx < static_cast<int>(saves.size())){
                    game.load_from_file(saves[idx]);
                    std::cout << "Game loaded successfully!\n";
                }
            } catch(...){std::cout << "Invalid choice.\n";}
        }
};
#endif