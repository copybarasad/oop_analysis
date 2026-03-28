#pragma once

#include "GameMaster.h"
#include "Input/KeyBinder.h"
#include "InputController.h"
#include "Visualizer/GameVisualizer.h"
#include "Visualizer/ConsoleRenderer.h"
#include "Spells/SpellShop.h"
#include "Animation/ASCIIAnimationPlayer.h"
#include "Logger/GameLogger.h"
#include <thread>
#include <chrono>
#include <vector>
#include <fstream>

class Game 
{
private:
    Player player;
    GameMaster master;
    ASCIIAnimationPlayer animPlayer;
    KeyBinder keyBinder;
    InputController<ConsoleInputSource> inputController;
    GameVisualizer<ConsoleRenderer> visualizer;
    
    void showKeyConfigMenu();
    
public:
    Game();
    void run();
    void mainMenu();
    void gameLoop();
};