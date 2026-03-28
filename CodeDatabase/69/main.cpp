#include "Game.h"
#include "GameController.h"
#include "ConsoleInputReader.h"
#include "ConsoleRenderer.h"
#include "GameView.h"
#include <iostream>

int main() {
    Game game(10, 10);
    ConsoleInputReader inputReader; //читаем консоль
    ConsoleRenderer renderer;
    GameView<ConsoleRenderer> view(renderer); //прослойка для того, чтобы считать рендерь
    GameController<ConsoleInputReader, GameView<ConsoleRenderer>> controller(game, inputReader, view);

    inputReader.LoadConfig("keybinding.txt");

    controller.Run();
    
    return 0;
}


//g++ -o game *.cpp
//rm -f game *.o
//g++ -std=c++20 -O2 ./*.cpp ./Commands/*.cpp -o game
