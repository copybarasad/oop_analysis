#include "game.h"
#include "user_input.h"
#include "view.h"
#include "print.h"

int main(){
    Game<User_input, Game_view<Console_renderer>> game;
    
    try{
        game.start_game();
        game.main_loop();
    }catch(ExitException& e){
        std::cout << e.what();
        return 0;
    }
    return 0;
}