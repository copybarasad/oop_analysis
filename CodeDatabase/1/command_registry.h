// command_registry.h
#pragma once

#include <map>
#include <string>
#include <vector>

class Command;

class CommandRegistry {
private:
    std::map<std::string, Command*> commandMap;  

    void initializeCommands();  

public:
    CommandRegistry();
    ~CommandRegistry();
    
    // Запрещаем копирование (из-за владения сырыми указателями)
    CommandRegistry(const CommandRegistry&) = delete;
    CommandRegistry& operator=(const CommandRegistry&) = delete;

    CommandRegistry(CommandRegistry&& other) noexcept;
    CommandRegistry& operator=(CommandRegistry&& other) noexcept;
    
    Command* getCommand(const std::string& input) const;
    bool isValidCommand(const std::string& input) const;
    Command* getCommandByName(const std::string& command_name) const;

    const std::map<std::string, Command*>& getCommandMap() const { return commandMap; }
    
    std::vector<std::pair<std::string, std::string>> getCommandsForDisplay() const;
    std::map<std::string, std::vector<std::pair<std::string, std::string>>> 
    getCommandsByCategory() const;
};