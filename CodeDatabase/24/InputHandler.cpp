#include "InputHandler.hpp"
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>

// Получение команды от пользователя
Command InputHandler::getCommand() {
    std::string line;
    if (!std::getline(std::cin, line)) {
        return Command{Command::Type::NONE, Pos(0, 0)};
    }

    return parseInput(line);
}

// Преобразование пользовательского ввода в игровую команду
Command InputHandler::parseInput(const std::string& line) const {
    // Удаление пробелов в начале и конце строки
    auto isSpace = [](unsigned char ch){ return std::isspace(ch); };
    std::string trimmedLine = line;
    trimmedLine.erase(trimmedLine.begin(), std::find_if(trimmedLine.begin(), trimmedLine.end(), 
                     [isSpace](unsigned char ch){ return !isSpace(ch); }));
    trimmedLine.erase(std::find_if(trimmedLine.rbegin(), trimmedLine.rend(), 
                     [isSpace](unsigned char ch){ return !isSpace(ch); }).base(), trimmedLine.end());

    if (trimmedLine.empty()) {
        return Command{Command::Type::NONE, Pos(0, 0)};
    }

    // Приведение к нижнему регистру
    std::string input;
    input.reserve(trimmedLine.size());
    for (char ch : trimmedLine) {
        input.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(ch))));
    }

    Command cmd{Command::Type::NONE, Pos(0, 0)};

    // Обработка команд движения
    if (input == "w") { cmd.type = Command::Type::MOVE; cmd.direction = Pos(0, -1); }
    else if (input == "s") { cmd.type = Command::Type::MOVE; cmd.direction = Pos(0, 1); }
    else if (input == "a") { cmd.type = Command::Type::MOVE; cmd.direction = Pos(-1, 0); }
    else if (input == "d") { cmd.type = Command::Type::MOVE; cmd.direction = Pos(1, 0); }

    // Обработка команд атаки
    else if (input == "i") { cmd.type = Command::Type::ATTACK; cmd.direction = Pos(0, -1); }
    else if (input == "k") { cmd.type = Command::Type::ATTACK; cmd.direction = Pos(0, 1); }
    else if (input == "j") { cmd.type = Command::Type::ATTACK; cmd.direction = Pos(-1, 0); }
    else if (input == "l") { cmd.type = Command::Type::ATTACK; cmd.direction = Pos(1, 0); }

    // Обработка применения заклинаний
    else if (input == "u") { cmd.type = Command::Type::CAST_SPELL; cmd.spellIndex = 0; }
    else if (input == "y") { cmd.type = Command::Type::CAST_SPELL; cmd.spellIndex = 1; }

    // Прочие команды
    else if (input == "c") { cmd.type = Command::Type::SWITCH_COMBAT_MODE; }
    else if (input == "q") { cmd.type = Command::Type::QUIT; }

    // Выбор пунктов меню (цифры 0-9)
    else if (input == "1") { cmd.type = Command::Type::MENU_SELECT; cmd.menuIndex = 0; }
    else if (input == "2") { cmd.type = Command::Type::MENU_SELECT; cmd.menuIndex = 1; }
    else if (input == "3") { cmd.type = Command::Type::MENU_SELECT; cmd.menuIndex = 2; }
    else if (input == "4") { cmd.type = Command::Type::MENU_SELECT; cmd.menuIndex = 3; }
    else if (input == "5") { cmd.type = Command::Type::MENU_SELECT; cmd.menuIndex = 4; }
    else if (input == "6") { cmd.type = Command::Type::MENU_SELECT; cmd.menuIndex = 5; }
    else if (input == "7") { cmd.type = Command::Type::MENU_SELECT; cmd.menuIndex = 6; }
    else if (input == "8") { cmd.type = Command::Type::MENU_SELECT; cmd.menuIndex = 7; }
    else if (input == "9") { cmd.type = Command::Type::MENU_SELECT; cmd.menuIndex = 8; }
    else if (input == "0") { cmd.type = Command::Type::MENU_SELECT; cmd.menuIndex = 9; }

    // Команды управления игрой
    else if (input == "save") { cmd.type = Command::Type::SAVE_GAME; }
    else if (input == "m") { cmd.type = Command::Type::MAIN_MENU; }

    else {
        cmd.type = Command::Type::NONE;
    }
    
    std::cout << "\n\n";
    return cmd;
}
