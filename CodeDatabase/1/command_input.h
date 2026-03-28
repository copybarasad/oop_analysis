#pragma once
#include "command_registry.h"
#include <string>
#include "key_bindings_config.h"  

class EventSystem;

class CommandInput {
private:
    CommandRegistry commandRegistry;
    KeyBindingsConfig keyBindings;
    EventSystem* eventSystem_;  
    
public:
    CommandInput(EventSystem* event_system = nullptr);
    Command* getNextCommand();
    Command* processInput(const std::string& input);
 
    bool isValidInput(const std::string& input) const;
    std::map<std::string, std::string> getCurrentBindings() const;
    std::vector<std::string> getValidationErrors() const;

    const CommandRegistry& getCommandRegistry() const { return commandRegistry; }
    
    void setEventSystem(EventSystem* event_system) { eventSystem_ = event_system; }
    
private:
    void logInputEvent(const std::string& event_type, const std::string& details);
};