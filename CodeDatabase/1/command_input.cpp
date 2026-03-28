#include "command_input.h"
#include "event_system.h"
#include "game_events.h"
#include "command.h"  
#include <iostream>

CommandInput::CommandInput(EventSystem* event_system) 
    : commandRegistry(), eventSystem_(event_system) {
    
    std::cout << "\n=== ЗАГРУЗКА КОНФИГУРАЦИИ УПРАВЛЕНИЯ ===\n";
    
    if (eventSystem_) {
        keyBindings.loadFromFile("key_bindings.cfg", eventSystem_);
    } else {
        keyBindings.loadFromFile("key_bindings.cfg");
    }

    auto bindings = keyBindings.getKeyBindings();
    std::cout << "Загружено " << bindings.size() << " привязок:\n";
    for (const auto& binding : bindings) {
        std::string key_display = (binding.first == " ") ? "[ПРОБЕЛ]" : binding.first;
        std::cout << "  " << key_display << " → " << binding.second << "\n";
    }
    std::cout << "=====================================\n\n";
}

Command* CommandInput::getNextCommand() {
    logInputEvent("INPUT", "Ожидание ввода команды...");
    
    std::string input;
    std::getline(std::cin, input);
    
    return processInput(input);
}

Command* CommandInput::processInput(const std::string& input) {
    logInputEvent("INPUT", "Получен ввод: '" + input + "'");
    
    if (input.empty()) {
        logInputEvent("ERROR", "Пустой ввод от пользователя");
        std::cout << "Пустой ввод. Используйте H для помощи.\n";
        return nullptr;
    }
    std::string command_name = keyBindings.getCommandForKey(input);
    
    if (command_name.empty()) {
        logInputEvent("ERROR", "Неизвестная клавиша: '" + input + "'");
        std::cout << "Неизвестная клавиша: '" << input << "'\n";
        std::cout << "Используйте H для помощи\n";
        
        return nullptr;
    }
    
    Command* command = commandRegistry.getCommandByName(command_name);
    
    if (!command) {
        logInputEvent("ERROR", "Команда '" + command_name + "' не найдена в реестре");
        std::cout << "Внутренняя ошибка: команда '" << command_name << "' не найдена\n";
        return nullptr;
    }
    logInputEvent("COMMAND", "Распознана команда: " + command_name + 
                 " (" + command->getDescription() + ")");
    
    return command;
}

bool CommandInput::isValidInput(const std::string& input) const {
    bool valid = !keyBindings.getCommandForKey(input).empty();
    
    if (eventSystem_) {
        std::string message = "Проверка ввода '" + input + "': " + 
                             (valid ? "ВАЛИДЕН" : "НЕВАЛИДЕН");
        GeneralEvent event("VALIDATION", message);
        eventSystem_->notify(event);
    }
    
    return valid;
}

std::map<std::string, std::string> CommandInput::getCurrentBindings() const {
    return keyBindings.getKeyBindings();
}

std::vector<std::string> CommandInput::getValidationErrors() const {
    return keyBindings.getValidationErrors();
}
void CommandInput::logInputEvent(const std::string& event_type, const std::string& details) {
    if (eventSystem_) {
        GeneralEvent event(event_type, "[INPUT] " + details);
        eventSystem_->notify(event);
    }
}