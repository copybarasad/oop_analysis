#include "InputConfig.h"
#include <vector>
#include "../Controllers/FileWrapper.h"
#include "../Constants.h"
#include "../Exceptions/ConfigException.h"
#include <iostream>
#include <cctype>

void InputConfig::setDefaultConfig(){
    this->charToCommand.clear();
    
    charToCommand['W'] = ActionType::MoveUp;
    charToCommand['A'] = ActionType::MoveLeft;
    charToCommand['S'] = ActionType::MoveDown;
    charToCommand['D'] = ActionType::MoveRight;
    charToCommand['F'] = ActionType::Attack;
    charToCommand['T'] = ActionType::Switch;
    charToCommand['P'] = ActionType::Pass;
    charToCommand['Q'] = ActionType::Surrender;
    charToCommand['I'] = ActionType::Info;
    charToCommand['E'] = ActionType::UseSpell;
    charToCommand['B'] = ActionType::BuySpell;
    charToCommand['Z'] = ActionType::Save;
    charToCommand['X'] = ActionType::Load;
}

bool InputConfig::checkConfig() const{
    std::map<char, ActionType> keyCheck;
    std::map<ActionType, char> actionCheck;
    
    for (auto [key, action] : this->charToCommand) {
        if (keyCheck.find(key) != keyCheck.end() || !std::isalnum(key)) {
            return false;
        }
        keyCheck[key] = action;
        
        if (actionCheck.find(action) != actionCheck.end()) {
            return false;
        }
        actionCheck[action] = key;
    }
    
    std::vector<ActionType> requiredActions = {
        ActionType::MoveUp, ActionType::MoveLeft,
        ActionType::MoveDown, ActionType::MoveRight,
        ActionType::Attack, ActionType::Switch,
        ActionType::Pass, ActionType::Surrender,
        ActionType::Info, ActionType::UseSpell,
        ActionType::BuySpell, ActionType::Save,
        ActionType::Load
    };
    
    for (auto action : requiredActions) {
        if (actionCheck.find(action) == actionCheck.end()) {
            return false;
        }
    }
    
    return true;
}

void InputConfig::setConfig(){
    std::map<std::string, ActionType> stringToAction = {
        {"Move Up", ActionType::MoveUp},
        {"Move Down", ActionType::MoveDown},
        {"Move Left", ActionType::MoveLeft},
        {"Move Right", ActionType::MoveRight},
        {"Attack", ActionType::Attack},
        {"Load", ActionType::Load},
        {"Save", ActionType::Save},
        {"Info", ActionType::Info},
        {"Surrender", ActionType::Surrender},
        {"Switch", ActionType::Switch},
        {"Pass", ActionType::Pass},
        {"Use Spell", ActionType::UseSpell},
        {"Buy Spell", ActionType::BuySpell}
    };

    FileWrapper configFile = FileWrapper(KEYBOARD_SETTINGS_FILE_PATH + ".json", Mode::Read);
    nlohmann::json config = configFile.read();
    this->charToCommand.clear();

    for (auto [actionStr, key] : config.items()){
        if (!key.is_string()){
            throw ConfigFileException("Key is not a string");
        }
        std::string keyStr = key.get<std::string>();
        if (keyStr.length() != 1){
            throw ConfigFileException("Key is not a char");
        }
        char keyUp = std::toupper(static_cast<char>(keyStr[0]));
        if (stringToAction.find(actionStr) == stringToAction.end()){
            throw ConfigFileException("Unknown option");
        }
        this->charToCommand[keyUp] = stringToAction[actionStr];
    }

    if (!this->checkConfig()){
        throw ConfigFileException("Wrong config");
    }
}

void InputConfig::setup(){
    try{
        this->setConfig();
    }
    catch(ConfigFileException& e){
        std::cout << e.what() << std::endl;
        this->setDefaultConfig();
    }
    catch(std::exception& e){
        std::cout << ConfigFileException(e.what()).what() << std::endl;
        this->setDefaultConfig();
    }
}

ActionType InputConfig::getAction(char key) const {
    auto it = charToCommand.find(key);
    if (it != charToCommand.end()) {
        return it->second;
    }
    throw NoKeyInConfigException();
}

std::map<char, ActionType> InputConfig::getCharToCommand() const{
    return this->charToCommand;
}