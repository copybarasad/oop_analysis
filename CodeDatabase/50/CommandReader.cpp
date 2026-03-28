#include "CommandReader.h"
#include <fstream>
#include <cctype>
#include <algorithm>

CommandReader::CommandReader(const std::string& configFile) {
    if (!loadFromFile(configFile)) {
        std::cout << "Используется управление по умолчанию.\n";
        setDefaultBindings();
    }
}

void CommandReader::setDefaultBindings() {
    keyToCommand.clear();
    keyToCommand['W'] = CommandType::MoveUp;
    keyToCommand['S'] = CommandType::MoveDown;
    keyToCommand['A'] = CommandType::MoveLeft;
    keyToCommand['D'] = CommandType::MoveRight;
    keyToCommand['E'] = CommandType::Attack;
    keyToCommand['C'] = CommandType::CastSpell;
    keyToCommand['V'] = CommandType::SaveGame;
    keyToCommand['L'] = CommandType::LoadGame;
    keyToCommand['N'] = CommandType::NewGame;
    keyToCommand['Q'] = CommandType::QuitGame;
    keyToCommand['B'] = CommandType::ExitToMenu;
    keyToCommand['M'] = CommandType::SwitchAttackType;
}

Command CommandReader::readCommand() {
    char ch = 0;
    std::cin >> ch;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    ch = static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));

    auto it = keyToCommand.find(ch);
    if (it == keyToCommand.end()){
        std::cout << "Неизвестная команда: '" << ch << "'.\n";
        Command cmd;
        cmd.type = CommandType::None;
        cmd.rawKey = ch;
        return cmd;

    }


    Command cmd;
    cmd.type = it->second;
    cmd.rawKey = ch;
    return cmd;
    
}

void CommandReader::printBindingsHelp() const {
    std::cout << "Управление (текущие привязки):\n";
    for (const auto& [key, cmd] : keyToCommand){
        std::cout << "  " << key << " - " << commandToName(cmd) << "\n";
    }
}
void CommandReader::printGameBindings() const {
    std::cout << "Команды игры:\n";

    for (const auto& [key, cmd] : keyToCommand){
        switch (cmd){
            case CommandType::MoveUp:
            case CommandType::MoveDown:
            case CommandType::MoveLeft:
            case CommandType::MoveRight:
            case CommandType::Attack:
            case CommandType::CastSpell:
            case CommandType::SaveGame:
            case CommandType::LoadGame:
            case CommandType::ExitToMenu:
            case CommandType::SwitchAttackType:
                std::cout << "  " << key << " - " << commandToName(cmd) << "\n";
                break;
            default:
                break; // ост команды скипаем
        }
    }
}

void CommandReader::printMenuBindings() const {
    std::cout << "Команды меню:\n";

    for (const auto& [key, cmd] : keyToCommand){
        switch (cmd){
            case CommandType::NewGame:
            case CommandType::LoadGame:
            case CommandType::QuitGame:
                std::cout << "  " << key << " - " << commandToName(cmd) << "\n";
                break;
            default:
                break; // ост команды скипаем
        }
    }
}




std::string CommandReader::trim(const std::string& s){
    size_t start = 0;
    while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start]))){
        ++start;
    } 
    size_t end = s.size();
    while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1]))){
        --end;
    } 
    return s.substr(start, end - start);
}

CommandType CommandReader::commandFromName(const std::string& nameRaw){
    std::string name = nameRaw;

    if (name == "MOVE_UP")      return CommandType::MoveUp;
    if (name == "MOVE_DOWN")    return CommandType::MoveDown;
    if (name == "MOVE_LEFT")    return CommandType::MoveLeft;
    if (name == "MOVE_RIGHT")   return CommandType::MoveRight;
    if (name == "ATTACK")       return CommandType::Attack;
    if (name == "CAST")         return CommandType::CastSpell;
    if (name == "SAVE")         return CommandType::SaveGame;
    if (name == "NEW_GAME")  return CommandType::NewGame;
    if (name == "QUIT_GAME") return CommandType::QuitGame;
    if (name == "LOAD")         return CommandType::LoadGame;
    if (name == "EXIT")         return CommandType::ExitToMenu;
    if (name == "SWITCH_ATTACK") return CommandType::SwitchAttackType;


    return CommandType::None;
}

std::string CommandReader::commandToName(CommandType cmd) {
    switch (cmd) {
        case CommandType::MoveUp:      return "Движение вверх";
        case CommandType::MoveDown:    return "Движение вниз";
        case CommandType::MoveLeft:    return "Движение влево";
        case CommandType::MoveRight:   return "Движение вправо";
        case CommandType::Attack:      return "Атака";
        case CommandType::CastSpell:   return "Каст заклинания";
        case CommandType::SaveGame:    return "Сохранение";
        case CommandType::LoadGame:    return "Загрузка";
        case CommandType::NewGame:  return "Новая игра";
        case CommandType::QuitGame: return "Выход";
        case CommandType::ExitToMenu:  return "Выход в главное меню";
        case CommandType::SwitchAttackType: return "Смена типа атаки";
        default:                       return "Неизвестная команда";
    }
}

bool CommandReader::loadFromFile(const std::string& filename) {
    std::ifstream in(filename);
    if (!in) {
        std::cout << "Не удалось открыть файл управления '" << filename
                  << "'.\n";
        return false;
    }

    std::map<char, CommandType> tmpKeyToCmd;
    std::map<CommandType, char> tmpCmdToKey;

    std::string line;
    while (std::getline(in, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#')
            continue;

        auto pos = line.find('=');
        if (pos == std::string::npos){
            std::cout << "Некорректная строка в файле управления: " << line << "\n";
            return false;
        }

        std::string nameStr = trim(line.substr(0, pos));
        std::string keyStr  = trim(line.substr(pos + 1));

        if (keyStr.empty()) {
            std::cout << "Пустая клавиша для команды " << nameStr << "\n";
            return false;
        }

        char key = keyStr[0];
        key = static_cast<char>(std::toupper(static_cast<unsigned char>(key)));

        CommandType cmd = commandFromName(nameStr);
        if (cmd == CommandType::None){
            std::cout << "Неизвестное имя команды в config: " << nameStr << "\n";
            return false;
        }

        if (tmpKeyToCmd.count(key)) {
            std::cout << "На клавишу " << key << " назначено более одной команды.\n";
            return false;
        }
        if (tmpCmdToKey.count(cmd)){
            std::cout << "Команда " << nameStr << " назначена на несколько клавиш.\n";
            return false;
        }

        tmpKeyToCmd[key] = cmd;
        tmpCmdToKey[cmd] = key;
    }

    // проверяем, что все команды присутствуют
    CommandType required[] = {
        CommandType::MoveUp,
        CommandType::MoveDown,
        CommandType::MoveLeft,
        CommandType::MoveRight,
        CommandType::Attack,
        CommandType::CastSpell,
        CommandType::SaveGame,
        CommandType::LoadGame,
        CommandType::ExitToMenu,
        CommandType::SwitchAttackType,
        CommandType::NewGame,
        CommandType::QuitGame   
    };

    for (CommandType cmd : required) {
        if (!tmpCmdToKey.count(cmd)) {
            std::cout << "В файле управления отсутствует команда: "
                      << commandToName(cmd) << "\n";
            return false;
        }
    }

    keyToCommand = std::move(tmpKeyToCmd);
    std::cout << "Управление успешно прочитано из '" << filename << "'.\n";
    return true;
}
