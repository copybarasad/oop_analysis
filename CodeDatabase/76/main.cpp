#include <iostream>
#include <ctime>
#include <cstdlib>
#include "Game.h"
#include "GameController.h"
#include "GameRenderer.h"
#include "ConsoleInputProvider.h"
#include "ConsoleRenderStrategy.h"
#include "GameCallbacksConfig.h"
#include "ExtendedGame.h"
#include <chrono>  
#include <thread>  

using namespace std;
int main() {
    srand((time(0)));
    
    ExtendedGame game;
    bool gameRunning = true;
    
    ConsoleInputProvider inputProvider;
    GameController<ConsoleInputProvider> controller(inputProvider);
    GameCallbacksConfig callbacks = GameCallbacksConfig::createDefaultCallbacks(game, gameRunning);
    callbacks.applyToController(controller);
    GameRenderer<ConsoleRenderStrategy> renderer(&game);
    
    controller.displayCurrentControls();
    
    cout << "\n=== STARTING GAME ===" << endl;
    cout << "Press Enter to begin..." << endl;
    cin.get();
    
    game.Initialize();  
    
    while (gameRunning && game.IsRunning() && game.GetPlayer().IsAlive_player()) {
        renderer.onGameStateChanged();
        
        if (!game.GetEnemy().IsAlive_enemy()) {
            game.HandleLevelCompletion();
            continue;
        }
        
        if (!game.GetPlayer().IsAlive_player()) {
            cout << "\n=== GAME OVER ===" << endl;
            break;
        }
        
        controller.processInput();
    }
    cout << "\nPress Enter to exit..." << endl;
    cin.get();
    
    return 0;
}