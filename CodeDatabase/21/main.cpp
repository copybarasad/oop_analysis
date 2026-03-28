#include <iostream>
#include <unistd.h>
#include "Game.hpp"
#include "./visual/IVisualizer.hpp"
#include "./visual/NCURSESVisualizer.hpp"
#include "./visual/NCURSESRender.hpp"
#include "./visual/Visualizer.hpp"
#include "InputController.hpp"
#include "AudioPlayer.hpp"
#include "KeyboardLayout.hpp"
#include "CommandFactory.hpp"
#include "GameManager.hpp"


int main() {
    NCURSESRender render;
    InputController inputController;
    KeyboardLayout keyboardLayout{"../settings/keyboard.cfg"};
    CommandFactory<InputController> commandInterpreter{inputController, keyboardLayout};
    Game game;
    GameManager gameManager{commandInterpreter, game};
    Visualizer gameVisualizer{render, &game};
    AudioPlayer music;

    GameState currentState = GameState::MainMenu;
    GameState prevState = GameState::MainMenu;
    while (currentState != GameState::Exit) {
        gameManager.recieveAndExecuteCommand(currentState, prevState);
        gameVisualizer.display(currentState);
    }
    music.stop();
    return 0;
}
