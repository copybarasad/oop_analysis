#include "game_coordinator_template.h"
#include "command_reader.h"
#include <iostream>

template<typename CommandHandlerType, typename GameType, typename RendererType>
GameCoordinatorTemplate<CommandHandlerType, GameType, RendererType>::GameCoordinatorTemplate(
    GameType& game,
    CommandHandlerType& command_handler,  
    std::unique_ptr<GameVisualizer<RendererType>> visualizer)
    : game_(game), 
      command_handler_(command_handler), 
      visualizer_(std::move(visualizer)) {
    
    if (!visualizer_) {
        visualizer_ = std::make_unique<GameVisualizer<RendererType>>(
            std::make_unique<RendererType>());
    }
}

template<typename CommandHandlerType, typename GameType, typename RendererType>
void GameCoordinatorTemplate<CommandHandlerType, GameType, RendererType>::Run() {
    visualizer_->VisualizeWelcome(); 
    
    bool game_loaded = AttemptLoadGame();
    int start_type = 0;
    
    if (!game_loaded) {
        game_.SetLevel(1);
        game_.RestartGame();
        start_type = 1;
    } else {
        start_type = 0;  
    }

    while (true) {
        if (start_type != 0 || !game_loaded) {
            visualizer_->VisualizeLevelStart(game_.GetLevel(), game_.GetLevel());  
            game_.StartLevel(start_type);
            start_type = 0;  
        }
        game_loaded = false;

        while (true) {
            visualizer_->VisualizeGameState(game_); 

            std::unique_ptr<Command> cmd = command_handler_.GetPlayerCommand();
            if (!cmd) {
                continue;  
            }

            if (!cmd->IsValid()) {
                std::cout << "Неверная команда!\n";
                continue;
            }

            cmd->Execute(game_);

            game_.EnemiesTurn();

            if (game_.IsLevelComplete()) {
                int next_level = game_.GetLevel() + 1;
                
                if (next_level > GameSession::MAX_LEVELS) {
                    game_.HandleVictory();
                    return;
                }
                
                game_.SetLevel(next_level);
                break;
            }

            if (game_.IsGameOver()) {
                visualizer_->VisualizeGameResult(game_);  
                
                char choice;
                while (true) {
                    std::cout << "\nНачать заново? (y/n): ";
                    std::cin >> choice;
                    command_handler_.ClearInputBuffer();

                    if (choice == 'n') {
                        std::cout << "Спасибо за игру!\n";
                        return;  
                    } else if (choice == 'y') {
                        game_.RestartGame();
                        game_.SetLevel(1);
                        game_.StartLevel(2);  
                        
                        start_type = 0;
                        break; 
                    } else {
                        std::cout << "Неверная команда! Введите 'y' или 'n'.\n";
                    }
                }
                continue;  
            }
        }
    }
}

template<typename CommandHandlerType, typename GameType, typename RendererType>
bool GameCoordinatorTemplate<CommandHandlerType, GameType, RendererType>::AttemptLoadGame() {
    if (!SaveManager::SaveExists()) {
        return false;
    }

    std::cout << "Загрузить сохранение? (y/n): ";
    char choice;
    std::cin >> choice;
    command_handler_.ClearInputBuffer();

    if (choice == 'y') {
        int loaded_level;
        if (game_.LoadGame(loaded_level)) {
            std::cout << "Игра загружена! Уровень: " << loaded_level << "\n";
            
            constexpr int MAX_LEVELS = 3;
            
            if (loaded_level > MAX_LEVELS) {
                std::cout << "Сохранение с будущего уровня. Новая игра.\n";
                return false;
            }
            return true;
        } else {
            std::cout << "Не удалось загрузить сохранение. Начинается новая игра.\n";
        }
    }
    return false;
}

template class GameCoordinatorTemplate<CommandReader, GameSession, ConsoleRenderer>;
