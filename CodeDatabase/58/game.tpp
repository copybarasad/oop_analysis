#pragma once

#include "game.hpp"
#include "keybindings_manager.hpp"

template<typename Controller, typename Visualizer>
void Game::run(Controller& controller, Visualizer& visualizer,
    int cnt_enemy, int cnt_enemy_build, int cnt_enemy_tower, int required_quantity){

    KeybindingsManager tmp_manager;
    if (!tmp_manager.config_file_exists()){
        std::string setup = controller.get_input("keybindings config file not found. Setup keybindings? (Y/n): ");
        std::transform(setup.begin(), setup.end(), setup.begin(), ::tolower);

        if (setup != "n"){
            controller.setup_keybindings_interactive(visualizer);
        }
    }

    std::string input = controller.get_input("start new game? (Y/n): ");

    std::transform(input.begin(), input.end(), input.begin(), ::tolower);
    init_world(cnt_enemy, cnt_enemy_build, cnt_enemy_tower, required_quantity, 1);

    if (input == "n"){
        Game load_game = sm.load_game(*this);
        *this = std::move(load_game);
    }
    World* snapshot_world = new World(*world);

    while(is_active){
        visualizer.render_player_stats(world->get_player(), level);
        visualizer.render_world(world);
        if (is_player_turn && !world->get_player().has_slowed()){
            if (controller.process_turn(visualizer)){
                continue;
            }

            is_player_turn = false;

            if(!world->has_eny_enemies()){
                visualizer.render_final_output(world->get_player().is_alive(), world->get_player().get_points());

                std::string text = "level: " + std::to_string(level) + " complete";
                visualizer.render_msg(text);

                std::string next = controller.get_input("start next level? (y/N): ");
                std::transform(next.begin(), next.end(), next.begin(), ::tolower);

                if(next != "y"){
                    is_active = false;
                } else{
                    gen_new_level(snapshot_world, cnt_enemy, cnt_enemy_build, cnt_enemy_tower, required_quantity);
                    level++;

                    delete snapshot_world;
                    snapshot_world = new World(*world);
                    is_player_turn = true;
                }
            }            
        } else{
            process_enemy();
            if(world->get_player().has_slowed()){
                process_enemy();
                world->get_player().switch_slowed();
            }
            is_player_turn = true;

            if (world->get_player().get_hp() == 0){
                visualizer.render_msg("You died!");
                std::string choice = controller.get_input("Try again? (y/N): ");
                std::transform(choice.begin(), choice.end(), choice.begin(), ::toupper);

                if (choice == "Y" || choice == "YES"){
                    delete world;
                    world = new World(*snapshot_world);

                    is_active = true;
                    is_player_turn = true;
                    cooldown_build = 0;
                } else{
                    is_active = false;
                    visualizer.render_final_output(world->get_player().is_alive(), world->get_player().get_points());
                }
            }
        }
    }
    delete snapshot_world;
}