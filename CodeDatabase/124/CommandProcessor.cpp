#include "CommandProcessor.h"
#include "ConcreteCommands.h"
#include <iostream>

CommandProcessor::CommandProcessor() {
    registerAllCommands();
}

void CommandProcessor::registerCommand(std::unique_ptr<IGameCommand> command) {
    if (command) {
        commandRegistry[command->getName()] = std::move(command);
    }
}

void CommandProcessor::registerAllCommands() { 
    registerCommand(std::make_unique<MoveUpCommand>());
    registerCommand(std::make_unique<MoveDownCommand>());
    registerCommand(std::make_unique<MoveLeftCommand>());
    registerCommand(std::make_unique<MoveRightCommand>());
    registerCommand(std::make_unique<AttackCommand>());
    registerCommand(std::make_unique<SwitchAttackCommand>());
    registerCommand(std::make_unique<CastSpellCommand>());
    registerCommand(std::make_unique<PrevSpellCommand>());
    registerCommand(std::make_unique<NextSpellCommand>());
    registerCommand(std::make_unique<SaveGameCommand>());
    registerCommand(std::make_unique<LoadGameCommand>());
    registerCommand(std::make_unique<BuySpellCommand>());
    registerCommand(std::make_unique<OpenShopCommand>());
}

IGameCommand* CommandProcessor::getCommand(const std::string& name) const {
    auto it = commandRegistry.find(name);
    return (it != commandRegistry.end()) ? it->second.get() : nullptr;
}
 
std::unique_ptr<IGameCommand> CommandProcessor::createCommand(char key) const {
    std::string commandName = keybindings.getCommand(key);
    if (commandName.empty()) {
        return nullptr;
    }
    
    IGameCommand* command = getCommand(commandName);
    if (command) {
        return command->clone();  
    }
    
    return nullptr;
}

IGameCommand* CommandProcessor::getCommandByKey(char key) const {
    std::string commandName = keybindings.getCommand(key);
    if (commandName.empty()) {
        return nullptr;
    }
    return getCommand(commandName);
}

bool CommandProcessor::processKey(char key, Player& player, GameField& field, Enemy& enemy, 
                                 EnemyManager& enemyManager, LevelManager& levelManager) {
    IGameCommand* command = getCommandByKey(key);
    if (command) {
        std::cout << "Выполняется команда: " << command->getDescription() << std::endl;
        bool result = command->execute(player, field, enemy, enemyManager, levelManager);
        return result && command->consumesTurn();
    }
    return false;
}

void CommandProcessor::printAvailableCommands() const {
    std::cout << "\n=== ДОСТУПНЫЕ КОМАНДЫ ===\n";
     
    std::unordered_map<std::string, std::vector<char>> commandToKeys;
    
    for (char c = 'a'; c <= 'z'; c++) {
        std::string cmd = keybindings.getCommand(c);
        if (!cmd.empty()) {
            commandToKeys[cmd].push_back(c);
        }
    }
     
    for (const auto& [cmdName, cmdPtr] : commandRegistry) {
        if (cmdPtr) {
            std::cout << cmdPtr->getDescription() << " - ";
             
            auto it = commandToKeys.find(cmdName);
            if (it != commandToKeys.end() && !it->second.empty()) {
                for (size_t i = 0; i < it->second.size(); i++) {
                    if (i > 0) std::cout << "/";
                    std::cout << "'" << it->second[i] << "'";
                }
            } else {
                std::cout << "нет привязки";
            }
            
            std::cout << " [" << (cmdPtr->consumesTurn() ? "ход" : "нет хода") << "]\n";
        }
    }
    std::cout << "========================\n";
}

bool CommandProcessor::reloadKeybindings() {
    return keybindings.loadFromFile();
} 