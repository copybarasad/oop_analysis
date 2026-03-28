#ifndef GAME_LOOPER_HPP
#define GAME_LOOPER_HPP
#include "game_controller.hpp"
#include "game_display.hpp"
#include "console_input.hpp"
#include "commands.hpp"
#include "game_renderer.hpp"
#include <memory>
#include <sstream>

template <typename T_input>
class Game_looper{
    private:
        Game_controller& game;
        Renderer_interface& renderer;
        T_input input;
        bool ask_yes_no(const std::string& str){
            bool flag = true;
            while(flag){
                std::cout << str;
                std::string line;
                std::getline(std::cin, line);
                std::istringstream iss(line);
                char choice;
                if(!(iss >> choice) || (iss >> std::ws).peek() != EOF){
                    std::cout << "Invalid input. Please enter 'Y' or 'N'.\n";
                    continue;
                }
                choice = std::tolower(static_cast<unsigned char>(choice));
                if(choice == 'y') return true;
                if(choice == 'n') return false;
                std::cout << "Invalid input. Please enter 'Y' or 'N'.\n";
            }
            return false;
        }
    public:
        Game_looper(Game_controller& g, Renderer_interface& r): game(g), renderer(r){}
        void run(){
            bool flag = true;
            renderer.show_help();
            renderer.render(game);
            while(flag){
                if(game.is_game_finished()){
                    game.on_game_over(game.get_enemies().empty());
                    renderer.show_game_over(game);
                    return;
                }
                bool is_help = false, is_quit = false;
                int hand_size = game.get_player().get_hand().size();
                auto command = input.get_next_command(is_help, is_quit, hand_size);
                if(is_quit) return;
                if(is_help){
                    renderer.show_help();
                    renderer.render(game);
                    continue;
                }
                if(!command){
                    std::cout << "Unknown command. Enter H for help.\n";
                    continue;
                }
                command->execute(game);
                if(game.is_level_completed()){
                    renderer.render(game);
                    std::cout << "\nLevel " << game.get_current_level() << " completed!\n";
                    std::cout << "Choose an upgrade:\n";
                    std::cout << "1) +5 to your damage (melee and ranged)\n";
                    std::cout << "2) +5 to all spell damage\n";
                    std::string choice;
                    while(flag){
                        std::cout << "Enter 1 or 2: ";
                        std::getline(std::cin, choice);
                        if(choice == "1"){
                            game.get_player().upgrade_damage(5);
                            std::cout << "Your damage increased by 5!\n";
                            break;
                        }
                        else if(choice == "2"){
                            game.get_player().upgrade_spell_damage(5);
                            std::cout << "All your spells now deal +5 damage!\n";
                            break;
                        }
                    }
                    if(ask_yes_no("Proceed to next level? (Y/N): ")) game.proceed_to_next_level();
                    else return;
                    renderer.render(game);
                    continue;
                }
                renderer.render(game);
            }
        }
};
#endif