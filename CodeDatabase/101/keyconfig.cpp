#include "keyconfig.h"
#include <unordered_map>
#include <QKeyEvent>


KeyConfig* KeyConfig::instance = nullptr;

KeyConfig::KeyConfig() : configValid(false) {
    resetToDefault();
}

KeyConfig& KeyConfig::getInstance() {
    if (!instance) {
        instance = new KeyConfig();
    }
    return *instance;
}


std::string KeyConfig::trim(const std::string& str) const{
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

std::string KeyConfig::toLower(const std::string& str) const{
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return result;
}

int KeyConfig::stringToKeyCode(const std::string& keyStr) const {
    std::string lowerKey = toLower(trim(keyStr));

    // Простейший mapping
    if (lowerKey == "left") return Qt::Key_Left;
    if (lowerKey == "right") return Qt::Key_Right;
    if (lowerKey == "up") return Qt::Key_Up;
    if (lowerKey == "down") return Qt::Key_Down;
    if (lowerKey == "space") return Qt::Key_Space;
    if (lowerKey == "shift") return Qt::Key_Shift;
    if (lowerKey == "escape") return Qt::Key_Escape;
    if (lowerKey == "1") return Qt::Key_1;
    if (lowerKey == "2") return Qt::Key_2;
    if (lowerKey == "3") return Qt::Key_3;
    if (lowerKey == "4") return Qt::Key_4;

    // Буквы
    if (lowerKey == "a") return Qt::Key_A;
    if (lowerKey == "b") return Qt::Key_B;
    if (lowerKey == "c") return Qt::Key_C;
    if (lowerKey == "d") return Qt::Key_D;
    if (lowerKey == "e") return Qt::Key_E;
    if (lowerKey == "f") return Qt::Key_F;
    if (lowerKey == "g") return Qt::Key_G;
    if (lowerKey == "h") return Qt::Key_H;
    if (lowerKey == "i") return Qt::Key_I;
    if (lowerKey == "j") return Qt::Key_J;
    if (lowerKey == "k") return Qt::Key_K;
    if (lowerKey == "l") return Qt::Key_L;
    if (lowerKey == "m") return Qt::Key_M;
    if (lowerKey == "n") return Qt::Key_N;
    if (lowerKey == "o") return Qt::Key_O;
    if (lowerKey == "p") return Qt::Key_P;
    if (lowerKey == "q") return Qt::Key_Q;
    if (lowerKey == "r") return Qt::Key_R;
    if (lowerKey == "s") return Qt::Key_S;
    if (lowerKey == "t") return Qt::Key_T;
    if (lowerKey == "u") return Qt::Key_U;
    if (lowerKey == "v") return Qt::Key_V;
    if (lowerKey == "w") return Qt::Key_W;
    if (lowerKey == "x") return Qt::Key_X;
    if (lowerKey == "y") return Qt::Key_Y;
    if (lowerKey == "z") return Qt::Key_Z;

    return -1;
}

std::string KeyConfig::keyCodeToString(int keyCode) const{
    static const std::unordered_map<int, std::string> reverseKeyMap = {
        {0x01000000, "Escape"},
        {0x20, "Space"},
        {0x01000020, "Shift"},
        {0x01000012, "Left"},
        {0x01000014, "Right"},
        {0x01000013, "Up"},
        {0x01000015, "Down"},
        {0x31, "1"},
        {0x32, "2"},
        {0x33, "3"},
        {0x34, "4"},
        {0x41, "A"},
        {0x44, "D"},
        {0x57, "W"},
        {0x53, "S"},
        {0x01000004, "Enter"}
    };

    auto it = reverseKeyMap.find(keyCode);
    if (it != reverseKeyMap.end()) {
        return it->second;
    }

    if (keyCode >= 0x41 && keyCode <= 0x5A) {
        return std::string(1, static_cast<char>(keyCode));
    }

    return "Unknown";
}

bool KeyConfig::isValidKeyCode(int keyCode) {
    return keyCode > 0;
}

bool KeyConfig::loadConfig(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл конфигурации: " << filename << std::endl;
        resetToDefault(); // Если файла нет - полный дефолт
        return false;
    }

    std::map<CommandType, int> tempKeyBindings;
    std::map<int, CommandType> tempReverseBindings;

    std::string line;
    int lineNumber = 0;

    while (std::getline(file, line)) {
        lineNumber++;
        line = trim(line);

        if (line.empty() || line[0] == '#') {
            continue;
        }

        size_t delimiterPos = line.find('=');
        if (delimiterPos == std::string::npos) {
            std::cerr << "Ошибка в строке " << lineNumber
                      << ": неверный формат (ожидается '=')" << std::endl;
            continue;
        }

        std::string commandStr = trim(line.substr(0, delimiterPos));
        std::string keyStr = trim(line.substr(delimiterPos + 1));

        size_t commentPos = keyStr.find('#');
        if (commentPos != std::string::npos) {
            keyStr = trim(keyStr.substr(0, commentPos));
        }

        CommandType command = CommandType(-1);
        for (auto cmd : getAllCommands()) {
            if (toLower(commandStr) == toLower(commandToString(cmd))) {
                command = cmd;
                break;
            }
        }

        if (static_cast<int>(command) == -1) {
            std::cerr << "Ошибка в строке " << lineNumber
                      << ": неизвестная команда '" << commandStr << "'" << std::endl;
            continue;
        }

        int keyCode = stringToKeyCode(keyStr);
        if (!isValidKeyCode(keyCode)) {
            std::cerr << "Ошибка в строке " << lineNumber
                      << ": неизвестная клавиша '" << keyStr << "'" << std::endl;
            continue;
        }

        if (tempReverseBindings.find(keyCode) != tempReverseBindings.end()) {
            std::cerr << "Ошибка в строке " << lineNumber
                      << ": клавиша '" << keyStr
                      << "' уже назначена для другой команды" << std::endl;
            continue;
        }

        if (tempKeyBindings.find(command) != tempKeyBindings.end()) {
            std::cerr << "Ошибка в строке " << lineNumber
                      << ": команда '" << commandStr
                      << "' уже имеет назначенную клавишу" << std::endl;
            continue;
        }

        tempKeyBindings[command] = keyCode;
        tempReverseBindings[keyCode] = command;
    }

    file.close();


    keyBindings = tempKeyBindings;
    reverseBindings = tempReverseBindings;


    std::map<CommandType, int> defaultBindings = getDefaultBindings();

    for (const auto& defaultPair : defaultBindings) {
        CommandType cmd = defaultPair.first;

        if (keyBindings.find(cmd) == keyBindings.end()) {
            int defaultKey = defaultPair.second;

            if (reverseBindings.find(defaultKey) == reverseBindings.end()) {
                keyBindings[cmd] = defaultKey;
                reverseBindings[defaultKey] = cmd;
                std::cout << "Добавлено из дефолта: "
                          << commandToString(cmd) << " = "
                          << keyCodeToString(defaultKey) << std::endl;
            }
        }
    }

    configValid = true;

    std::cout << "Конфигурация загружена. Всего привязок: "
              << keyBindings.size() << std::endl;
    return true;
}


bool KeyConfig::saveConfig(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Не удалось создать файл конфигурации: " << filename << std::endl;
        return false;
    }

    file << "# Конфигурация управления игрой\n";
    file << "# Формат: КОМАНДА=КЛАВИША\n";
    file << "# Доступные клавиши: Escape, Space, Shift, Left, Right, Up, Down, 1-4, A-Z\n\n";

    for (const auto& binding : keyBindings) {
        std::string commandStr = commandToString(binding.first);
        std::string keyStr = keyCodeToString(binding.second);

        std::transform(commandStr.begin(), commandStr.end(), commandStr.begin(),
                       [](unsigned char c){ return std::toupper(c); });

        file << commandStr << "=" << keyStr << "\n";
    }

    file.close();
    std::cout << "Конфигурация сохранена в файл: " << filename << std::endl;
    return true;
}


void KeyConfig::resetToDefault() {
    keyBindings.clear();
    reverseBindings.clear();

    keyBindings[CommandType::MOVE_LEFT] = 0x01000012;  // Left
    keyBindings[CommandType::MOVE_RIGHT] = 0x01000014; // Right
    keyBindings[CommandType::MOVE_UP] = 0x01000013;    // Up
    keyBindings[CommandType::MOVE_DOWN] = 0x01000015;  // Down
    keyBindings[CommandType::ATTACK_CLOSE] = 0x20;     // Space
    keyBindings[CommandType::SWITCH_WEAPON] = 0x01000020; // Shift
    keyBindings[CommandType::PAUSE_GAME] = 0x01000000; // Escape
    keyBindings[CommandType::NEW_GAME] = 0x31;         // 1
    keyBindings[CommandType::LOAD_GAME] = 0x32;        // 2
    keyBindings[CommandType::QUIT_GAME] = 0x33;        // 3
    keyBindings[CommandType::CONTINUE_LEVEL] = 0x34;   // 4

    for (const auto& binding : keyBindings) {
        reverseBindings[binding.second] = binding.first;
    }

    configValid = true;
    std::cout << "Используется конфигурация по умолчанию" << std::endl;
}


CommandType KeyConfig::getCommand(int keyCode) const {
    auto it = reverseBindings.find(keyCode);
    if (it != reverseBindings.end()) {
        return it->second;
    }
    return static_cast<CommandType>(-1);
}


int KeyConfig::getKey(CommandType command) const {
    auto it = keyBindings.find(command);
    if (it != keyBindings.end()) {
        return it->second;
    }
    return -1;
}


void KeyConfig::setKeyBinding(CommandType command, int keyCode) {

    int oldKey = getKey(command);
    if (oldKey != -1) {
        reverseBindings.erase(oldKey);
    }

    CommandType oldCommand = getCommand(keyCode);
    if (static_cast<int>(oldCommand) != -1) {
        keyBindings.erase(oldCommand);
    }

    keyBindings[command] = keyCode;
    reverseBindings[keyCode] = command;
}

std::string KeyConfig::commandToString(CommandType command) {
    static const std::unordered_map<CommandType, std::string> commandMap = {
        {CommandType::MOVE_LEFT, "move_left"},
        {CommandType::MOVE_RIGHT, "move_right"},
        {CommandType::MOVE_UP, "move_up"},
        {CommandType::MOVE_DOWN, "move_down"},
        {CommandType::ATTACK_CLOSE, "attack_close"},
        {CommandType::SWITCH_WEAPON, "switch_weapon"},
        {CommandType::PAUSE_GAME, "pause_game"},
        {CommandType::NEW_GAME, "new_game"},
        {CommandType::LOAD_GAME, "load_game"},
        {CommandType::QUIT_GAME, "quit_game"},
        {CommandType::CONTINUE_LEVEL, "continue_level"},
        {CommandType::MOUSE_CLICK, "mouse_click"},
        {CommandType::MOUSE_DROP, "mouse_drop"},
        {CommandType::START_DRAG, "start_drag"}
    };

    auto it = commandMap.find(command);
    if (it != commandMap.end()) {
        return it->second;
    }
    return "unknown";
}


std::vector<CommandType> KeyConfig::getAllCommands() {
    return {
        CommandType::MOVE_LEFT,
        CommandType::MOVE_RIGHT,
        CommandType::MOVE_UP,
        CommandType::MOVE_DOWN,
        CommandType::ATTACK_CLOSE,
        CommandType::SWITCH_WEAPON,
        CommandType::PAUSE_GAME,
        CommandType::NEW_GAME,
        CommandType::LOAD_GAME,
        CommandType::QUIT_GAME,
        CommandType::CONTINUE_LEVEL,
        CommandType::MOUSE_CLICK,
        CommandType::MOUSE_DROP,
        CommandType::START_DRAG
    };
}

std::vector<std::string> KeyConfig::validateConfig() const {
    std::vector<std::string> errors;

    std::map<int, int> keyCount;
    for (const auto& binding : keyBindings) {
        keyCount[binding.second]++;
        if (keyCount[binding.second] > 1) {
            std::string keyName = keyCodeToString(binding.second);
            errors.push_back("Клавиша '" + keyName + "' назначена на несколько команд");
        }
    }


    std::vector<CommandType> requiredCommands = {
        CommandType::MOVE_LEFT,
        CommandType::MOVE_RIGHT,
        CommandType::MOVE_UP,
        CommandType::MOVE_DOWN,
        CommandType::ATTACK_CLOSE,
        CommandType::SWITCH_WEAPON,
        CommandType::PAUSE_GAME,
        CommandType::NEW_GAME,
        CommandType::LOAD_GAME,
        CommandType::QUIT_GAME,
        CommandType::CONTINUE_LEVEL
    };

    for (auto cmd : requiredCommands) {
        if (keyBindings.find(cmd) == keyBindings.end()) {
            errors.push_back("Команда '" + commandToString(cmd) + "' не настроена");
        }
    }

    return errors;
}

std::map<CommandType, int> KeyConfig::getDefaultBindings() const {
    std::map<CommandType, int> defaults;

    defaults[CommandType::MOVE_LEFT] = Qt::Key_Left;
    defaults[CommandType::MOVE_RIGHT] = Qt::Key_Right;
    defaults[CommandType::MOVE_UP] = Qt::Key_Up;
    defaults[CommandType::MOVE_DOWN] = Qt::Key_Down;
    defaults[CommandType::ATTACK_CLOSE] = Qt::Key_Space;
    defaults[CommandType::SWITCH_WEAPON] = Qt::Key_Shift;
    defaults[CommandType::PAUSE_GAME] = Qt::Key_Escape;
    defaults[CommandType::NEW_GAME] = Qt::Key_1;
    defaults[CommandType::LOAD_GAME] = Qt::Key_2;
    defaults[CommandType::QUIT_GAME] = Qt::Key_3;
    defaults[CommandType::CONTINUE_LEVEL] = Qt::Key_4;


    defaults[CommandType::MOUSE_CLICK] = -1;
    defaults[CommandType::MOUSE_DROP] = -1;
    defaults[CommandType::START_DRAG] = -1;

    return defaults;
}

bool KeyConfig::isValid() const { return configValid; }
