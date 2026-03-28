#pragma once
#include "Command.hpp"
#include <string>

// Класс для обработки пользовательского ввода с консоли для использования в шаблонном GameController
class InputHandler {
public:
    // Получение команды от пользователя
    Command getCommand();
    
private:
    // Преобразование строки ввода в команду
    Command parseInput(const std::string& input) const;
};
