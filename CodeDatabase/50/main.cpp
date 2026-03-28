#include "GameController.h"
#include "CommandReader.h"
#include "RendererTemplate.h"
#include "ConsolePolicy.h"

int main() {
    Game game;

    CommandReader input;                        
    RendererTemplate<ConsolePolicy> renderer;   

    GameController<CommandReader> controller(game, input); 

    controller.runGame(renderer);               

    return 0;
}