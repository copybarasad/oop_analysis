#pragma once
#include <type_traits>
#include "Command.hpp"

// Шаблонный класс для управления вводом пользователя
// Работает с любфм типом, у которого есть метод Command getCommand()
template<typename InputParserType>
class GameController {
    // Статическая проверка наличия метода getCommand()
    static_assert(std::is_same_v<
        decltype(std::declval<InputParserType>().getCommand()), 
        Command
    >, "InputParserType должен иметь метод Command getCommand()");
    
    InputParserType inputParser;
public:
    GameController() = default;
    // Получение команды от пользователя через парсер ввода
    Command getCommand() {
        return inputParser.getCommand();
    }
};
