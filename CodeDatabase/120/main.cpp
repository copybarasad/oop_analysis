#include "GameController.h"
#include "AttackCommand.h"
#include "MoveCommand.h"
#include "OpenHandCommand.h"
#include "OpenShopCommand.h"
#include "SaveCommand.h"
#include "LoadCommand.h"
#include "SwitchModeCommand.h"
#include "QuitCommand.h"
#include "ConsoleCommandReader.h"
#include "IGameRenderer.h"
#include "InputConfigLoader.h"

int main() {
    InputConfigLoader loader;
    loader.LoadConfig("controls.cfg"); 
    EventSystem events;
    if(loader.GetLogTarget() == LogTarget::File) {
        events.SetLogTarget(LogTarget::File, loader.GetLogFile());
    } else {
        events.SetLogTarget(LogTarget::Screen);
    }

    GameWorld world(20,20);
    GameLogic logic(world,events);

    CommandRegistry registry;

    auto controlMap = loader.GetMapping();


    std::unordered_map<std::string, std::function<std::unique_ptr<ICommand>()>> commandFactory = {
        {"MoveUp",      [](){ return std::make_unique<MoveCommand>(0,-1); }},
        {"MoveDown",    [](){ return std::make_unique<MoveCommand>(0,1); }},
        {"MoveLeft",    [](){ return std::make_unique<MoveCommand>(-1,0); }},
        {"MoveRight",   [](){ return std::make_unique<MoveCommand>(1,0); }},
        {"Attack",      [](){ return std::make_unique<AttackCommand>(); }},
        {"OpenShop",    [](){ return std::make_unique<OpenShopCommand>(); }},
        {"OpenHand",    [](){ return std::make_unique<OpenHandCommand>(); }},
        {"SwitchMode",  [](){ return std::make_unique<SwitchModeCommand>(); }},
        {"Save",        [](){ return std::make_unique<SaveCommand>(); }},
        {"Load",        [](){ return std::make_unique<LoadCommand>(); }},
        {"Quit",        [](){ return std::make_unique<QuitCommand>(); }}
    };


    for (const auto& [key, cmdName] : controlMap) {
        auto it = commandFactory.find(cmdName);
        if (it != commandFactory.end()) {
            registry.RegisterCommand(key, it->second);
        } else {
            std::cerr << "Warning: unknown command '" << cmdName 
                      << "' for key '" << key << "'\n";
        }
    }

    ConsoleCommandReader reader(controlMap);
    using Renderer = GameRenderer<ConsoleRenderer>;
    GameController<ConsoleCommandReader, Renderer> controller(world, logic, registry, reader);

    controller.Run();
    return 0;
}
