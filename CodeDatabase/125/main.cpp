#include <iostream>
#include <clocale>
#include "game_coordinator_template.h"
#include "command_reader.h"
#include "game_session.h"
#include "game_visualizer.h"
#include "console_renderer.h"
#include "command_factory.h"
#include "input_config.h"

int main() {
    try {
        setlocale(LC_ALL, "Russian");
        
        InputConfig config;
        bool config_loaded = config.LoadFromFile("controls.json");
        
        if (!config_loaded) {
            std::cout << "Запуск игры с настройками управления по умолчанию.\n";
        }
        
        InputConfig::SetInstance(config);
        
        CommandFactory::SetConfig(config);    
        
        GameSession game(20, 10);
        CommandReader command_reader;
        auto visualizer = std::make_unique<GameVisualizer<ConsoleRenderer>>(
            std::make_unique<ConsoleRenderer>());
        
        std::cout << "\n" << config.GetHelpString() << "\n";
        
        GameCoordinatorTemplate<CommandReader, GameSession, ConsoleRenderer> 
            coordinator(game, command_reader, std::move(visualizer));
        
        coordinator.Run();
        
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
