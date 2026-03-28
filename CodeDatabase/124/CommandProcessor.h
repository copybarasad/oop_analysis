#ifndef COMMAND_PROCESSOR_H
#define COMMAND_PROCESSOR_H

#include "IGameCommand.h"
#include "KeybindingsConfig.h"
#include <unordered_map>
#include <memory>
#include <string>

class CommandProcessor {
private:
    std::unordered_map<std::string, std::unique_ptr<IGameCommand>> commandRegistry;
    KeybindingsConfig keybindings;
    
public:
    CommandProcessor();
    
    void registerCommand(std::unique_ptr<IGameCommand> command);
    void registerAllCommands();
    
    IGameCommand* getCommand(const std::string& name) const;
     
    std::unique_ptr<IGameCommand> createCommand(char key) const;
     
    IGameCommand* getCommandByKey(char key) const;
    
    bool processKey(char key, Player& player, GameField& field, Enemy& enemy, 
                   EnemyManager& enemyManager, LevelManager& levelManager);
    
    void printAvailableCommands() const;
    bool reloadKeybindings();
    const KeybindingsConfig& getKeybindings() const { return keybindings; }
};

#endif 