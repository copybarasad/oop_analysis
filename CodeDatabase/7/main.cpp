#define NOMINMAX
#include <windows.h>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <memory>
#include <string>
#include <limits>

#include "Game.h"
#include "GameException.h"
#include "command_mapper.h"
#include "input_reader.h"
#include "fileinput.h"
#include "game_controller.h"
#include "game_vizualizator.h"
#include "console_renderer.h"
#include "color_vis.h"
#include "logger.h"

using namespace std;

template<typename ControllerType, typename VisualizationType>
void runGameWithController(Game& game, ControllerType& controller, VisualizationType& visualization) {
    bool running = true;
    while (running && game.isGameRunning()) {
        visualization.renderGame();
        visualization.showPrompt("Введите команду: ");
        running = controller.processInput(game, visualization);
        
        if (running) {
            game.processEnemyTurn();
            game.checkEndConditions();
            Sleep(400); 
        }
    }
    if (!game.isGameRunning()) {
        visualization.renderMessage("\n=== ИГРА ЗАВЕРШЕНА ===");
    }
}

int main(int argc, char* argv[]) {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    srand(static_cast<unsigned int>(time(0)));
    
    bool useColor = false;
    bool demoMode = false;
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "--color") useColor = true;
        else if (arg == "--demo") demoMode = true;
    }
    
    try {
        Logger* logger = new Logger(true, true, "game_log.txt");
        
        Game game;
        game.setLogger(logger);
        
        CommandMapper commandMapper;
        if (!commandMapper.initialize("controls.config")) {
            game.getLogger()->log("Файл конфигурации управления некорректен. Использованы настройки по умолчанию.", LogLevel::L_WARNING);
        }
        
        cout << "\n=== МЕНЮ ===" << endl;
        cout << "1. Новая игра\n2. Загрузить игру\n3. Демо-режим\n4. Выход\nВыбор: ";
        char choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        if (choice == '4') { delete logger; return 0; }

        if (choice == '2') {
            try { game.loadGame("savefile.txt"); } 
            catch (...) { cout << "Файл не найден. Новая игра..." << endl; game.setupLevel(); }
        } else if (choice == '3') {
            demoMode = true;
            game.setupLevel();
        } else {
            game.setupLevel();
        }
        
        if (demoMode) {
            DemoInput demoInput(cin, commandMapper);
            if (useColor) {
                GameVisualization<ColorfulRenderer> vis(&game);
                GameController<DemoInput, ColorfulRenderer> ctrl(demoInput);
                runGameWithController(game, ctrl, vis);
            } else {
                GameVisualization<ConsoleRenderer> vis(&game);
                GameController<DemoInput, ConsoleRenderer> ctrl(demoInput);
                runGameWithController(game, ctrl, vis);
            }
        } else {
            InputReader inputReader(cin, &commandMapper);
            if (useColor) {
                GameVisualization<ColorfulRenderer> vis(&game);
                GameController<InputReader, ColorfulRenderer> ctrl(inputReader);
                runGameWithController(game, ctrl, vis);
            } else {
                GameVisualization<ConsoleRenderer> vis(&game);
                GameController<InputReader, ConsoleRenderer> ctrl(inputReader);
                runGameWithController(game, ctrl, vis);
            }
        }
        
        delete logger; 

    } catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}