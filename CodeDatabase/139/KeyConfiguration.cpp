#include "KeyConfiguration.h"
#include <algorithm>
#include <unordered_set>
#include <iostream>


KeyConfiguration::ConfigurationWrapper::ConfigurationWrapper(const std::string& filename) : file(filename)
{
    if (!file.is_open())
        throw std::runtime_error(u8"[KeyConfiguration] Не удалось открыть файл для чтения: " + filename);
}


KeyConfiguration::ConfigurationWrapper::~ConfigurationWrapper()
{
    if (file.is_open())
        file.close();
}


KeyConfiguration::KeyConfiguration(const std::string& filename)
{
    try {
        ConfigurationWrapper reader = ConfigurationWrapper(filename);
        if (!reader.loadKeys(keyMap))
        {
            loadDefaultKeyMap();
            std::cout << u8"[KeyConfiguration] Некорректный формат. Дефолтные настройки загружены.\n";
        }
        else {
            std::cout << u8"[KeyConfiguration] Загружены пользовательские настройки.\n";
        }
    }
    catch (std::runtime_error& e) {
        std::cout << e.what() << std::endl;
        loadDefaultKeyMap();
    }


}


KeyCode KeyConfiguration::ConfigurationWrapper::parseAction(const std::string& action) const
{
    if (action == "MoveUp") return KeyCode::Up;
    if (action == "MoveDown") return KeyCode::Down;
    if (action == "MoveLeft") return KeyCode::Left;
    if (action == "MoveRight") return KeyCode::Right;
    if (action == "SwitchFightMode") return KeyCode::L;
    if (action == "SwitchSpeedMode") return KeyCode::LShift;
    if (action == "UseSpell") return KeyCode::M;
    if (action == "AttackUp") return KeyCode::W;
    if (action == "AttackDown") return KeyCode::S;
    if (action == "AttackLeft") return KeyCode::A;
    if (action == "AttackRight") return KeyCode::D;

    return KeyCode::Unknown;
}


KeyCode KeyConfiguration::ConfigurationWrapper::parseKeyCode(const std::string& key) const
{
    std::string keyLower = key;

    std::transform(keyLower.begin(), keyLower.end(), keyLower.begin(), ::tolower);

    // arrows
    if (keyLower == "up") return KeyCode::Up;
    if (keyLower == "down") return KeyCode::Down;
    if (keyLower == "left") return KeyCode::Left;
    if (keyLower == "right") return KeyCode::Right;

    // digits
    if (keyLower == "0" || keyLower == "num0") return KeyCode::Num0;
    if (keyLower == "1" || keyLower == "num1") return KeyCode::Num1;
    if (keyLower == "2" || keyLower == "num2") return KeyCode::Num2;
    if (keyLower == "3" || keyLower == "num3") return KeyCode::Num3;
    if (keyLower == "4" || keyLower == "num4") return KeyCode::Num4;
    if (keyLower == "5" || keyLower == "num5") return KeyCode::Num5;
    if (keyLower == "6" || keyLower == "num6") return KeyCode::Num6;
    if (keyLower == "7" || keyLower == "num7") return KeyCode::Num7;
    if (keyLower == "8" || keyLower == "num8") return KeyCode::Num8;
    if (keyLower == "9" || keyLower == "num9") return KeyCode::Num9;

    // letters A–Z
    if (keyLower == "a") return KeyCode::A;
    // BUY SPELL RESERVED
    //if (keyLower == "b") return KeyCode::B;
    //if (keyLower == "c") return KeyCode::C;
    if (keyLower == "d") return KeyCode::D;
    if (keyLower == "e") return KeyCode::E;
    if (keyLower == "f") return KeyCode::F;
    if (keyLower == "g") return KeyCode::G;
    if (keyLower == "h") return KeyCode::H;
    if (keyLower == "i") return KeyCode::I;
    if (keyLower == "j") return KeyCode::J;
    if (keyLower == "k") return KeyCode::K;
    if (keyLower == "l") return KeyCode::L;
    if (keyLower == "m") return KeyCode::M;
    if (keyLower == "n") return KeyCode::N;
    if (keyLower == "o") return KeyCode::O;
    if (keyLower == "p") return KeyCode::P;
    // QUIT RESERVED
    //if (keyLower == "q") return KeyCode::Q; 
    if (keyLower == "r") return KeyCode::R;
    if (keyLower == "s") return KeyCode::S;
    if (keyLower == "t") return KeyCode::T;
    if (keyLower == "u") return KeyCode::U;
    // BUY SPELL RESERVED
    //if (keyLower == "v") return KeyCode::V;
    if (keyLower == "w") return KeyCode::W;
    // BUY SPELL RESERVED
    //if (keyLower == "x") return KeyCode::X;
    if (keyLower == "y") return KeyCode::Y;
    // BUY SPELL RESERVED
    //if (keyLower == "z") return KeyCode::Z;

    // functional
    if (keyLower == "space") return KeyCode::Space;
    if (keyLower == "enter") return KeyCode::Enter;

    // PAUSE MENU RESERVED
    //if (keyLower == "esc" || s == "escape") return KeyCode::ESC;
    if (keyLower == "lshift" || keyLower == "shift") return KeyCode::LShift;
    if (keyLower == "rshift") return KeyCode::RShift;
    if (keyLower == "ctrl") return KeyCode::Ctrl;
    if (keyLower == "alt") return KeyCode::Alt;

    return KeyCode::Unknown;    
}


KeyCode KeyConfiguration::getKey(KeyCode key) const
{
    auto it = keyMap.find(key);
    if (it != keyMap.end())
        return it->second;
    // Если клавиши нет, то возвращаем ее же.
    return key;
}


bool KeyConfiguration::ConfigurationWrapper::loadKeys(std::unordered_map<KeyCode, KeyCode>& keyMap)
{
    keyMap.clear();

    nlohmann::json jsonFile;
    try {
        file >> jsonFile;
    }
    catch (...) {
        return false; // файл не JSON
    }

    if (!jsonFile.is_object())
        return false;

    // Временные множества для проверки ошибок
    std::unordered_set<KeyCode> usedKeys;
    std::unordered_set<KeyCode> usedActions;

    for (auto& [actionStr, keyStrJson] : jsonFile.items())
    {
        if (!keyStrJson.is_string())
            return false;

        std::string keyStr = keyStrJson.get<std::string>();

        // действие -> KeyCode
        KeyCode action = parseAction(actionStr);
        if (action == KeyCode::Unknown)
            return false;

        // клавиша -> KeyCode
        KeyCode key = parseKeyCode(keyStr);
        if (key == KeyCode::Unknown)
            return false;

        // Проверка: одно действие дважды
        if (usedActions.find(action) != usedActions.end())
        {
            return false;
        }

        usedActions.insert(action);

        // Проверка: две команды на одну клавишу
        if (usedKeys.find(key) != usedKeys.end())
            return false;

        usedKeys.insert(key);

        keyMap[key] = action;
    }

    return true;
}


void KeyConfiguration::loadDefaultKeyMap()
{
    keyMap.clear();

    keyMap[KeyCode::Up] = KeyCode::Up;             // MoveUp
    keyMap[KeyCode::Down] = KeyCode::Down;         // MoveDown
    keyMap[KeyCode::Left] = KeyCode::Left;         // MoveLeft
    keyMap[KeyCode::Right] = KeyCode::Right;       // MoveRight

    keyMap[KeyCode::L] = KeyCode::L;               // SwitchFightMode
    keyMap[KeyCode::LShift] = KeyCode::LShift;     // SwitchSpeedMode
    keyMap[KeyCode::M] = KeyCode::M;               // UseSpell

    keyMap[KeyCode::W] = KeyCode::W;               // AttackUp
    keyMap[KeyCode::S] = KeyCode::S;               // AttackDown
    keyMap[KeyCode::A] = KeyCode::A;               // AttackLeft
    keyMap[KeyCode::D] = KeyCode::D;               // AttackRight
}
