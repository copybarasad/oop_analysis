
#include <iostream>
#include "game.hpp"
#include "player.hpp"
#include "game_controller.hpp"
#include "game_visualizer.hpp"
#include "console_input_handler.hpp"
#include "console_renderer.hpp"

using namespace std;

int main(){
    Player* player_ptr = new Player(50, 50, TypeAttack::MELEE, 5);
    Game g(10, 10, player_ptr, 10);
    
    GameController<ConsoleInputHandler> controller(&g);
    GameVisualizer<ConsoleRenderer> visualizer(&g);
    
    g.run(controller, visualizer, 1, 1, 1, 5);
    return 0;
}