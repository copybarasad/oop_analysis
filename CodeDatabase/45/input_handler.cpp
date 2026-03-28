#include "input_handler.hpp"

void InputHandler::start_game() {
    std::cout << "Management: wasd. To switch combat types, press M (melee) or R(ranged).  In ranged mode, you cannot move, the arrow flies in the specified direction to the first obstacle.\n";

    std::cout << "New game (N) or Load saved game (L)?\n";

    Command com = inputMaster->get_char();
    std::string key = com.get_text();

    if (key == "n" || key == "N") {
        gc->new_game();
    } else if (key == "l" || key == "L") {
        std::cout << "Which file you want to load?\n";
        Command com = inputMaster->get_string();
        gc->load(com);
    }
    while (gc->game_active()) {
        com = inputMaster->get_char();
        if (com.get_text() == "p" || com.get_text() == "P") {
            pause();
        }
        gc->game_cycle(com);
        //gc->end_cycle();
        if (!gc->game_active()) {
            gc->end_cycle();
            com = inputMaster->get_char();
            if (com.get_text() == "e" || com.get_text() == "E") {
                break;
            } else if (com.get_text() == "n" || com.get_text() == "N") {
                gc->new_game();
            } else if (com.get_text() == "l" || com.get_text() == "L") {
                std::cout << "Which file you want to load?\n";
                gc->load(inputMaster->get_string());
            } else if (com.get_text() == "s" || com.get_text() == "S") {
                std::cout << "You want to gain 50 hp (1) or 20 damage (2)?\n";
                gc->next_level(inputMaster->get_char());
            }
        }
    }
}

void InputHandler::pause() {
    std::cout << "Game paused!\nResume (R), Save (S), Load (L)\n";
    Command com = inputMaster->get_char();
    while (com.get_text() != "r" && com.get_text() != "R") {
        if (com.get_text() == "s" || com.get_text() == "S") {
            std::cout << "In which file you want to save game?\n";
            gc->save(inputMaster->get_string());
            break;
        } else if (com.get_text() == "l" || com.get_text() == "L") {
            std::cout << "Which file you want to load?\n";
            gc->load(inputMaster->get_string());
            break;
        }
        com = inputMaster->get_char();
    }
}
