#include "headers/gameController.hpp"
#include "gui_headers/inputHandler.hpp"

#include <iostream>

int main(){
    try{
        Game game;
        GameController <InputHandler> controller;
        controller.run(game);
    }
    catch (const std::exception& e){
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}