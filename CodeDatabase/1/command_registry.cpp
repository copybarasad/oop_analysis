#include "command_registry.h"
#include "command.h"  
#include <iostream>

CommandRegistry::~CommandRegistry() {
    for (auto& pair : commandMap) {
        delete pair.second;
    }
}

void CommandRegistry::initializeCommands() {
    commandMap = {
        // Движение
        {"W", new MoveUpCommand()},
        {"S", new MoveDownCommand()},
        {"A", new MoveLeftCommand()},
        {"D", new MoveRightCommand()},
        
        // Боевые команды
        {" ", new AttackCommand()},           
        {"B", new SwitchCombatCommand()},
        {"F", new CastSpellCommand()},
        
        // UI команды
        {"I", new ShowStatusCommand()},
        {"M", new LookAroundCommand()},
        {"P", new ShowSpellsCommand()},
        {"H", new HelpCommand()},
        
        // Системные команды
        {"O", new SaveGameCommand()},
        {"L", new LoadGameCommand()},
        {"Q", new QuitCommand()}
    };
}

CommandRegistry::CommandRegistry() {
    initializeCommands();
}


Command* CommandRegistry::getCommand(const std::string& input) const {
    if (input.empty()) {
        return nullptr;
    }
    
    auto it = commandMap.find(input);
    if (it != commandMap.end()) {
        return it->second;
    }
    
    return nullptr;
}

bool CommandRegistry::isValidCommand(const std::string& input) const {
    return getCommand(input) != nullptr;
}

Command* CommandRegistry::getCommandByName(const std::string& command_name) const {
    for (const auto& pair : commandMap) {
        if (pair.second->getName() == command_name) {
            return pair.second;
        }
    }
    return nullptr;
}

// Получение команд для отображения
std::vector<std::pair<std::string, std::string>> 
CommandRegistry::getCommandsForDisplay() const {
    std::vector<std::pair<std::string, std::string>> result;
    
    // Движение
    result.push_back({"W", "Вверх"});
    result.push_back({"S", "Вниз"});
    result.push_back({"A", "Влево"});
    result.push_back({"D", "Вправо"});
    
    // Бой
    result.push_back({" ", "Атака (пробел)"});
    result.push_back({"B", "Сменить режим боя"});
    result.push_back({"F", "Применить заклинание"});
    
    // Информация
    result.push_back({"I", "Статус"});
    result.push_back({"M", "Карта"});
    result.push_back({"P", "Заклинания"});
    result.push_back({"H", "Помощь"});
    
    // Система
    result.push_back({"O", "Сохранить"});
    result.push_back({"L", "Загрузить"});
    result.push_back({"Q", "Выйти"});
    
    return result;
}

// Получение команд сгруппированных по категориям
std::map<std::string, std::vector<std::pair<std::string, std::string>>> 
CommandRegistry::getCommandsByCategory() const {
    std::map<std::string, std::vector<std::pair<std::string, std::string>>> categories;
    
    // Движение
    categories["🚶 ДВИЖЕНИЕ"] = {
        {"W", "Вверх"}, {"S", "Вниз"}, {"A", "Влево"}, {"D", "Вправо"}
    };
    
    // Бой
    categories["⚔️  БОЙ"] = {
        {" ", "Атака"}, {"B", "Сменить режим боя"}, {"F", "Применить заклинание"}
    };
    
    // Информация
    categories["📊 ИНФОРМАЦИЯ"] = {
        {"I", "Статус"}, {"M", "Карта"}, {"P", "Заклинания"}, {"H", "Помощь"}
    };
    
    
    // Система
    categories["💾 СИСТЕМА"] = {
        {"O", "Сохранить"}, {"L", "Загрузить"}, {"Q", "Выйти"}
    };
    
    return categories;
}

CommandRegistry::CommandRegistry(CommandRegistry&& other) noexcept 
    : commandMap(std::move(other.commandMap)) {
    other.commandMap.clear();  
}

CommandRegistry& CommandRegistry::operator=(CommandRegistry&& other) noexcept {
    if (this != &other) {
        for (auto& pair : commandMap) {
            delete pair.second;
        }

        commandMap = std::move(other.commandMap);
        other.commandMap.clear();
    }
    return *this;
}
