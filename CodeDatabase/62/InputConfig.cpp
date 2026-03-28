#include "InputConfig.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

// Вспомогательная функция для преобразования строки в Scancode
sf::Keyboard::Scancode stringToScancode(const std::string& str) {
    // Преобразуем в верхний регистр для сравнения
    std::string upper = str;
    std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);
    
    // Маппинг строк на Scancode
    if (upper == "A") return sf::Keyboard::Scancode::A;
    if (upper == "B") return sf::Keyboard::Scancode::B;
    if (upper == "C") return sf::Keyboard::Scancode::C;
    if (upper == "D") return sf::Keyboard::Scancode::D;
    if (upper == "E") return sf::Keyboard::Scancode::E;
    if (upper == "F") return sf::Keyboard::Scancode::F;
    if (upper == "G") return sf::Keyboard::Scancode::G;
    if (upper == "H") return sf::Keyboard::Scancode::H;
    if (upper == "I") return sf::Keyboard::Scancode::I;
    if (upper == "J") return sf::Keyboard::Scancode::J;
    if (upper == "K") return sf::Keyboard::Scancode::K;
    if (upper == "L") return sf::Keyboard::Scancode::L;
    if (upper == "M") return sf::Keyboard::Scancode::M;
    if (upper == "N") return sf::Keyboard::Scancode::N;
    if (upper == "O") return sf::Keyboard::Scancode::O;
    if (upper == "P") return sf::Keyboard::Scancode::P;
    if (upper == "Q") return sf::Keyboard::Scancode::Q;
    if (upper == "R") return sf::Keyboard::Scancode::R;
    if (upper == "S") return sf::Keyboard::Scancode::S;
    if (upper == "T") return sf::Keyboard::Scancode::T;
    if (upper == "U") return sf::Keyboard::Scancode::U;
    if (upper == "V") return sf::Keyboard::Scancode::V;
    if (upper == "W") return sf::Keyboard::Scancode::W;
    if (upper == "X") return sf::Keyboard::Scancode::X;
    if (upper == "Y") return sf::Keyboard::Scancode::Y;
    if (upper == "Z") return sf::Keyboard::Scancode::Z;
    if (upper == "ESCAPE") return sf::Keyboard::Scancode::Escape;
    if (upper == "SPACE") return sf::Keyboard::Scancode::Space;
    if (upper == "ENTER") return sf::Keyboard::Scancode::Enter;
    if (upper == "SHIFT") return sf::Keyboard::Scancode::LShift;
    if (upper == "CTRL") return sf::Keyboard::Scancode::LControl;
    if (upper == "ALT") return sf::Keyboard::Scancode::LAlt;
    
    // Цифры
    if (upper == "1" || upper == "NUM1") return sf::Keyboard::Scancode::Num1;
    if (upper == "2" || upper == "NUM2") return sf::Keyboard::Scancode::Num2;
    if (upper == "3" || upper == "NUM3") return sf::Keyboard::Scancode::Num3;
    if (upper == "4" || upper == "NUM4") return sf::Keyboard::Scancode::Num4;
    if (upper == "5" || upper == "NUM5") return sf::Keyboard::Scancode::Num5;
    if (upper == "6" || upper == "NUM6") return sf::Keyboard::Scancode::Num6;
    if (upper == "7" || upper == "NUM7") return sf::Keyboard::Scancode::Num7;
    if (upper == "8" || upper == "NUM8") return sf::Keyboard::Scancode::Num8;
    if (upper == "9" || upper == "NUM9") return sf::Keyboard::Scancode::Num9;
    if (upper == "0" || upper == "NUM0") return sf::Keyboard::Scancode::Num0;
    if (upper == "-" || upper == "MINUS" || upper == "HYPHEN") return sf::Keyboard::Scancode::Hyphen;
    
    // Стрелки
    if (upper == "UP" || upper == "ARROWUP") return sf::Keyboard::Scancode::Up;
    if (upper == "DOWN" || upper == "ARROWDOWN") return sf::Keyboard::Scancode::Down;
    if (upper == "LEFT" || upper == "ARROWLEFT") return sf::Keyboard::Scancode::Left;
    if (upper == "RIGHT" || upper == "ARROWRIGHT") return sf::Keyboard::Scancode::Right;
    
    return sf::Keyboard::Scancode::Unknown;
}

// Вспомогательная функция для преобразования строки в EventType
EventType stringToEventType(const std::string& str) {
    std::string upper = str;
    std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);
    
    if (upper == "MOVERIGHT" || upper == "MOVE_RIGHT") return EventType::MovePlayer;
    if (upper == "MOVELEFT" || upper == "MOVE_LEFT") return EventType::MovePlayer;
    if (upper == "MOVEUP" || upper == "MOVE_UP") return EventType::MovePlayer;
    if (upper == "MOVEDOWN" || upper == "MOVE_DOWN") return EventType::MovePlayer;
    if (upper == "CHANGEATTACKMODE" || upper == "CHANGE_ATTACK_MODE") return EventType::ChangePlayerAttackMode;
    if (upper == "SAVEGAME" || upper == "SAVE_GAME") return EventType::SaveGame;
    if (upper == "LOADGAME" || upper == "LOAD_GAME") return EventType::LoadGame;
    
    return EventType::MovePlayer; // По умолчанию
}

// Получает параметры для MovePlayer в зависимости от направления
void getMovePlayerArgs(const std::string& direction, std::vector<float>& floatArgs) {
    std::string upper = direction;
    std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);
    
    if (upper == "RIGHT" || upper == "MOVERIGHT" || upper == "MOVE_RIGHT") {
        floatArgs.push_back(1.0f);
        floatArgs.push_back(0.0f);
    } else if (upper == "LEFT" || upper == "MOVELEFT" || upper == "MOVE_LEFT") {
        floatArgs.push_back(-1.0f);
        floatArgs.push_back(0.0f);
    } else if (upper == "UP" || upper == "MOVEUP" || upper == "MOVE_UP") {
        floatArgs.push_back(0.0f);
        floatArgs.push_back(-1.0f);
    } else if (upper == "DOWN" || upper == "MOVEDOWN" || upper == "MOVE_DOWN") {
        floatArgs.push_back(0.0f);
        floatArgs.push_back(1.0f);
    }
}

std::map<EventType, sf::Keyboard::Scancode> InputConfig::getDefaultConfig() {
    std::map<EventType, sf::Keyboard::Scancode> config;
    // Для MovePlayer используем специальную обработку
    return config;
}

InputConfig::InputConfig() {
    setDefault();
}

bool InputConfig::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open config file: " << filename << ", using defaults" << std::endl;
        return false;
    }
    
    // Очищаем текущую конфигурацию
    keyToCommand_.clear();
    commandToKey_.clear();
    
    std::string line;
    std::map<sf::Keyboard::Scancode, std::string> tempKeyToCommand;
    std::map<std::string, sf::Keyboard::Scancode> tempCommandToKey;
    
    while (std::getline(file, line)) {
        // Пропускаем пустые строки и комментарии
        if (line.empty() || line[0] == '#') continue;
        
        std::istringstream iss(line);
        std::string command, key;
        
        if (iss >> command >> key) {
            sf::Keyboard::Scancode scancode = stringToScancode(key);
            if (scancode == sf::Keyboard::Scancode::Unknown) {
                std::cerr << "Error: Unknown key: " << key << std::endl;
                file.close();
                setDefault();
                return false;
            }
            
            // Проверяем, не назначена ли уже эта клавиша
            if (tempKeyToCommand.find(scancode) != tempKeyToCommand.end()) {
                std::cerr << "Error: Key " << key << " is already assigned to " << tempKeyToCommand[scancode] << std::endl;
                file.close();
                setDefault();
                return false;
            }
            
            // Проверяем, не назначена ли уже эта команда
            if (tempCommandToKey.find(command) != tempCommandToKey.end()) {
                std::cerr << "Error: Command " << command << " is already assigned to another key" << std::endl;
                file.close();
                setDefault();
                return false;
            }
            
            tempKeyToCommand[scancode] = command;
            tempCommandToKey[command] = scancode;
        }
    }
    
    file.close();
    
    // Преобразуем в CommandConfig
    for (const auto& pair : tempKeyToCommand) {
        CommandConfig config;
        std::string command = pair.second;
        
        if (command.find("MOVE") != std::string::npos) {
            config.eventType = EventType::MovePlayer;
            getMovePlayerArgs(command, config.floatArgs);
        } else if (command == "CHANGEATTACKMODE" || command == "CHANGE_ATTACK_MODE") {
            config.eventType = EventType::ChangePlayerAttackMode;
        } else if (command == "SAVEGAME" || command == "SAVE_GAME") {
            config.eventType = EventType::SaveGame;
        } else if (command == "LOADGAME" || command == "LOAD_GAME") {
            config.eventType = EventType::LoadGame;
        } else if (command == "USESPELL1" || command == "USE_SPELL_1") {
            config.eventType = EventType::UseSpellFromHand;
            config.intArgs.push_back(0);
        } else if (command == "USESPELL2" || command == "USE_SPELL_2") {
            config.eventType = EventType::UseSpellFromHand;
            config.intArgs.push_back(1);
        } else if (command == "USESPELL3" || command == "USE_SPELL_3") {
            config.eventType = EventType::UseSpellFromHand;
            config.intArgs.push_back(2);
        } else if (command == "USESPELL4" || command == "USE_SPELL_4") {
            config.eventType = EventType::UseSpellFromHand;
            config.intArgs.push_back(3);
        } else if (command == "USESPELL5" || command == "USE_SPELL_5") {
            config.eventType = EventType::UseSpellFromHand;
            config.intArgs.push_back(4);
        } else if (command == "BUYSHOP1" || command == "BUY_SHOP_1") {
            config.eventType = EventType::BuyShopItem;
            config.intArgs.push_back(0);
        } else if (command == "BUYSHOP2" || command == "BUY_SHOP_2") {
            config.eventType = EventType::BuyShopItem;
            config.intArgs.push_back(1);
        } else if (command == "BUYSHOP3" || command == "BUY_SHOP_3") {
            config.eventType = EventType::BuyShopItem;
            config.intArgs.push_back(2);
        } else if (command == "BUYSHOP4" || command == "BUY_SHOP_4") {
            config.eventType = EventType::BuyShopItem;
            config.intArgs.push_back(3);
        } else if (command == "BUYSHOP5" || command == "BUY_SHOP_5") {
            config.eventType = EventType::BuyShopItem;
            config.intArgs.push_back(4);
        } else if (command == "BUYSHOP6" || command == "BUY_SHOP_6") {
            config.eventType = EventType::BuyShopItem;
            config.intArgs.push_back(5);
        } else if (command == "ATTACKRIGHT" || command == "ATTACK_RIGHT") {
            config.eventType = EventType::AttackDirection;
            config.floatArgs.push_back(1.0f);
            config.floatArgs.push_back(0.0f);
        } else if (command == "ATTACKLEFT" || command == "ATTACK_LEFT") {
            config.eventType = EventType::AttackDirection;
            config.floatArgs.push_back(-1.0f);
            config.floatArgs.push_back(0.0f);
        } else if (command == "ATTACKUP" || command == "ATTACK_UP") {
            config.eventType = EventType::AttackDirection;
            config.floatArgs.push_back(0.0f);
            config.floatArgs.push_back(-1.0f);
        } else if (command == "ATTACKDOWN" || command == "ATTACK_DOWN") {
            config.eventType = EventType::AttackDirection;
            config.floatArgs.push_back(0.0f);
            config.floatArgs.push_back(1.0f);
        } else if (command == "SELECTBUTTON1" || command == "SELECT_BUTTON_1") {
            config.eventType = EventType::SelectButton;
            config.intArgs.push_back(0);
        } else if (command == "SELECTBUTTON2" || command == "SELECT_BUTTON_2") {
            config.eventType = EventType::SelectButton;
            config.intArgs.push_back(1);
        } else if (command == "SELECTBUTTON3" || command == "SELECT_BUTTON_3") {
            config.eventType = EventType::SelectButton;
            config.intArgs.push_back(2);
        } else if (command == "SELECTBUTTON4" || command == "SELECT_BUTTON_4") {
            config.eventType = EventType::SelectButton;
            config.intArgs.push_back(3);
        } else {
            std::cerr << "Error: Unknown command: " << command << std::endl;
            setDefault();
            return false;
        }
        
        keyToCommand_[pair.first] = config;
    }
    
    // Проверяем корректность
    if (!isValid()) {
        std::cerr << "Configuration is invalid, using defaults" << std::endl;
        setDefault();
        return false;
    }
    
    return true;
}

void InputConfig::setDefault() {
    keyToCommand_.clear();
    commandToKey_.clear();
    
    // MoveRight -> D
    CommandConfig moveRight;
    moveRight.eventType = EventType::MovePlayer;
    moveRight.floatArgs = {1.0f, 0.0f};
    keyToCommand_[sf::Keyboard::Scancode::D] = moveRight;
    
    // MoveLeft -> A
    CommandConfig moveLeft;
    moveLeft.eventType = EventType::MovePlayer;
    moveLeft.floatArgs = {-1.0f, 0.0f};
    keyToCommand_[sf::Keyboard::Scancode::A] = moveLeft;
    
    // MoveUp -> W
    CommandConfig moveUp;
    moveUp.eventType = EventType::MovePlayer;
    moveUp.floatArgs = {0.0f, -1.0f};
    keyToCommand_[sf::Keyboard::Scancode::W] = moveUp;
    
    // MoveDown -> S
    CommandConfig moveDown;
    moveDown.eventType = EventType::MovePlayer;
    moveDown.floatArgs = {0.0f, 1.0f};
    keyToCommand_[sf::Keyboard::Scancode::S] = moveDown;
    
    // ChangeAttackMode -> E
    CommandConfig changeAttackMode;
    changeAttackMode.eventType = EventType::ChangePlayerAttackMode;
    keyToCommand_[sf::Keyboard::Scancode::E] = changeAttackMode;
    
    // SaveGame -> M
    CommandConfig saveGame;
    saveGame.eventType = EventType::SaveGame;
    keyToCommand_[sf::Keyboard::Scancode::N] = saveGame;
    
    // LoadGame -> N
    CommandConfig loadGame;
    loadGame.eventType = EventType::LoadGame;
    keyToCommand_[sf::Keyboard::Scancode::M] = loadGame;
    
    // UseSpellFromHand 1-5 -> Num1-Num5
    for (int i = 0; i < 5; i++) {
        CommandConfig useSpell;
        useSpell.eventType = EventType::UseSpellFromHand;
        useSpell.intArgs.push_back(i);
        sf::Keyboard::Scancode key = static_cast<sf::Keyboard::Scancode>(
            static_cast<int>(sf::Keyboard::Scancode::Num1) + i
        );
        keyToCommand_[key] = useSpell;
    }
    
    // BuyShopItem 1-6 -> Num6, Num7, Num8, Num9, Num0, Hyphen
    for (int i = 0; i < 6; i++) {
        CommandConfig buyShop;
        buyShop.eventType = EventType::BuyShopItem;
        buyShop.intArgs.push_back(i);
        sf::Keyboard::Scancode key;
        if (i == 0) key = sf::Keyboard::Scancode::Num6;
        else if (i == 1) key = sf::Keyboard::Scancode::Num7;
        else if (i == 2) key = sf::Keyboard::Scancode::Num8;
        else if (i == 3) key = sf::Keyboard::Scancode::Num9;
        else if (i == 4) key = sf::Keyboard::Scancode::Num0;
        else key = sf::Keyboard::Scancode::Hyphen;
        keyToCommand_[key] = buyShop;
    }
    
    // AttackDirection -> Arrow keys
    CommandConfig attackRight;
    attackRight.eventType = EventType::AttackDirection;
    attackRight.floatArgs = {1.0f, 0.0f};
    keyToCommand_[sf::Keyboard::Scancode::Right] = attackRight;
    
    CommandConfig attackLeft;
    attackLeft.eventType = EventType::AttackDirection;
    attackLeft.floatArgs = {-1.0f, 0.0f};
    keyToCommand_[sf::Keyboard::Scancode::Left] = attackLeft;
    
    CommandConfig attackUp;
    attackUp.eventType = EventType::AttackDirection;
    attackUp.floatArgs = {0.0f, -1.0f};
    keyToCommand_[sf::Keyboard::Scancode::Up] = attackUp;
    
    CommandConfig attackDown;
    attackDown.eventType = EventType::AttackDirection;
    attackDown.floatArgs = {0.0f, 1.0f};
    keyToCommand_[sf::Keyboard::Scancode::Down] = attackDown;
    
    // SelectButton 1-4 -> Z, X, C, V
    CommandConfig selectButton1;
    selectButton1.eventType = EventType::SelectButton;
    selectButton1.intArgs.push_back(0);
    keyToCommand_[sf::Keyboard::Scancode::Z] = selectButton1;
    
    CommandConfig selectButton2;
    selectButton2.eventType = EventType::SelectButton;
    selectButton2.intArgs.push_back(1);
    keyToCommand_[sf::Keyboard::Scancode::X] = selectButton2;
    
    CommandConfig selectButton3;
    selectButton3.eventType = EventType::SelectButton;
    selectButton3.intArgs.push_back(2);
    keyToCommand_[sf::Keyboard::Scancode::C] = selectButton3;
    
    CommandConfig selectButton4;
    selectButton4.eventType = EventType::SelectButton;
    selectButton4.intArgs.push_back(3);
    keyToCommand_[sf::Keyboard::Scancode::V] = selectButton4;
}

const CommandConfig* InputConfig::getCommand(sf::Keyboard::Scancode key) const {
    auto it = keyToCommand_.find(key);
    if (it != keyToCommand_.end()) {
        return &it->second;
    }
    return nullptr;
}

bool InputConfig::isValid() const {
    // Проверяем, что все необходимые команды присутствуют
    // Для MovePlayer нужно проверить все 4 направления
    bool hasMoveRight = false, hasMoveLeft = false, hasMoveUp = false, hasMoveDown = false;
    bool hasChangeAttackMode = false, hasSaveGame = false, hasLoadGame = false;
    
    // Счетчики для проверки дубликатов направлений MovePlayer
    int moveRightCount = 0, moveLeftCount = 0, moveUpCount = 0, moveDownCount = 0;
    
    // Счетчики для других команд
    int changeAttackModeCount = 0, saveGameCount = 0, loadGameCount = 0;
    
    for (const auto& pair : keyToCommand_) {
        const CommandConfig& config = pair.second;
        if (config.eventType == EventType::MovePlayer) {
            if (config.floatArgs.size() >= 2) {
                if (config.floatArgs[0] > 0 && config.floatArgs[1] == 0) {
                    hasMoveRight = true;
                    moveRightCount++;
                } else if (config.floatArgs[0] < 0 && config.floatArgs[1] == 0) {
                    hasMoveLeft = true;
                    moveLeftCount++;
                } else if (config.floatArgs[0] == 0 && config.floatArgs[1] < 0) {
                    hasMoveUp = true;
                    moveUpCount++;
                } else if (config.floatArgs[0] == 0 && config.floatArgs[1] > 0) {
                    hasMoveDown = true;
                    moveDownCount++;
                }
            }
        } else if (config.eventType == EventType::ChangePlayerAttackMode) {
            hasChangeAttackMode = true;
            changeAttackModeCount++;
        } else if (config.eventType == EventType::SaveGame) {
            hasSaveGame = true;
            saveGameCount++;
        } else if (config.eventType == EventType::LoadGame) {
            hasLoadGame = true;
            loadGameCount++;
        }
    }
    
    // Проверяем, что все команды присутствуют
    if (!hasMoveRight || !hasMoveLeft || !hasMoveUp || !hasMoveDown ||
        !hasChangeAttackMode || !hasSaveGame || !hasLoadGame) {
        return false;
    }
    
    // Проверяем, что нет дубликатов (для каждой команды должна быть только одна клавиша)
    if (moveRightCount > 1 || moveLeftCount > 1 || moveUpCount > 1 || moveDownCount > 1 ||
        changeAttackModeCount > 1 || saveGameCount > 1 || loadGameCount > 1) {
        return false;
    }
    
    return true;
}
