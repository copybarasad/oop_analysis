#include "game.hpp"
#include <iostream>

Game::Game()
    : roster(5, level),
      field(roster)
{
}

void Game::loop() {
    while (player.alive() && (!roster.enemies.empty() || !roster.towers.empty())) {
        field.show(player);

        std::cout << "Action (wasd/m/c/b/q): ";
        char action;
        std::cin >> action;

        if (action == 'q') {
            std::cout << "Thanks for playing!\n";
            break;
        }

        bool turn_spent = false;

        if (action == 'c') {
            player.swap_weapon();
            std::cout << "Weapon changed to: " << player.get_weapon_name() << "\n";
            turn_spent = true;
        }
        else if (action == 'm') {
            if (player.handle.size() == 0) {
                std::cout << "No spells!\n";
                turn_spent = true;
            } else {
                player.handle.list_spells();
                int idx; std::cout << "Index: "; std::cin >> idx;
                if (idx >= 0 && idx < static_cast<int>(player.handle.size())) {
                    int tx, ty; std::cout << "Target x y: "; std::cin >> tx >> ty;
                    if (player.use_spell(idx, field, roster, tx, ty)) {
                        turn_spent = true;
                    }
                }
            }
        } else if (action == 'b'){
            player.get_spell();
            turn_spent = true;
        }
        else{
            int result = player.move(field.box, field.x, field.y, action);
            if (result == -1) {
                break;
            }
            if (result != 0) {
                roster.atack_enemy(player.damage, result);
            }
            turn_spent = true;
        }

        if (turn_spent) {
            roster.move_tower(field.box);
            roster.move_attack_towers(field, roster, player);
            roster.move_mob(field.box, player, field.x, field.y);
            roster.move_ally(field.box, field.x, field.y);
            field.remove_dead_entities(roster);
            roster.update();
        }
    }
}