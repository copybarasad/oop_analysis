#pragma once
#include "command_handler.h"
#include "command.h" 
#include "game_session.h"
#include "save_manager.h"  
#include "ui_manager.h" 
#include "game_visualizer.h"  
#include "console_renderer.h"     
#include <type_traits>
#include <memory> 

template<typename CommandHandlerType, typename GameType, typename RendererType = ConsoleRenderer>
class GameCoordinatorTemplate {
    static_assert(std::is_base_of_v<ICommandHandler, CommandHandlerType>, 
                  "CommandHandlerType must inherit from ICommandHandler");

public:
    GameCoordinatorTemplate(
        GameType& game,
        CommandHandlerType& command_handler,  
        std::unique_ptr<GameVisualizer<RendererType>> visualizer = nullptr);
    
    void Run();

private:
    GameType& game_; 
    CommandHandlerType command_handler_;
    std::unique_ptr<GameVisualizer<RendererType>> visualizer_;
    
    bool AttemptLoadGame();
};

extern template class GameCoordinatorTemplate<CommandReader, GameSession, ConsoleRenderer>;