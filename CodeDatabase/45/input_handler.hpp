#ifndef INPUT_HANDLER_HPP
#define INPUT_HANDLER_HPP
#include <memory>
#include <iostream>
#include "save_loader.hpp"
#include "command.hpp"
#include <string>
#include "input_master.hpp"
#include "visual_master.hpp"
#include "game_controller.hpp"
#include "save_loader.hpp"
#include "game_drawer.hpp"

class InputHandler {
private:
    std::unique_ptr<InputMaster> inputMaster;
    GameController* gc;
    void pause();
public:
    InputHandler(std::unique_ptr<InputMaster> im, GameController* game_contr_ptr) 
        : inputMaster(std::move(im)), gc(game_contr_ptr) {}
    void start_game();
};
#endif