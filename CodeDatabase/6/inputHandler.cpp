#include "../gui_headers/inputHandler.hpp"

InputHandler::InputHandler(){
    setupDefaultBindings();
}

void InputHandler::setupDefaultBindings(){
    stateKeyBindings[GameState::MAIN_MENU] = {
        {sf::Keyboard::Num1, CommandType::NEW_GAME},
        {sf::Keyboard::Num2, CommandType::CONTINUE_GAME},
        {sf::Keyboard::Num3, CommandType::EXIT}
    };
    
    stateKeyBindings[GameState::PLAYING] = {
        {sf::Keyboard::Up, CommandType::MOVE_UP},
        {sf::Keyboard::Down, CommandType::MOVE_DOWN},
        {sf::Keyboard::Left, CommandType::MOVE_LEFT},
        {sf::Keyboard::Right, CommandType::MOVE_RIGHT},
        {sf::Keyboard::F, CommandType::TOGGLE_FIGHT},
        {sf::Keyboard::A, CommandType::SHOW_ATTACK_RADIUS},
        {sf::Keyboard::B, CommandType::BUY_SPELL},
        {sf::Keyboard::S, CommandType::SAVE_GAME},
        {sf::Keyboard::Num1, CommandType::CAST_SPELL_1},
        {sf::Keyboard::Num2, CommandType::CAST_SPELL_2},
        {sf::Keyboard::Num3, CommandType::CAST_SPELL_3},
        {sf::Keyboard::Num4, CommandType::CAST_SPELL_4},
        {sf::Keyboard::Num5, CommandType::CAST_SPELL_5},
        {sf::Keyboard::Num6, CommandType::CAST_SPELL_6},
        {sf::Keyboard::Escape, CommandType::CANCEL}
    };
    
    stateKeyBindings[GameState::LEVEL_COMPLETE] = {
        {sf::Keyboard::Num1, CommandType::CONTINUE_NEXT_LEVEL},
        {sf::Keyboard::Num2, CommandType::MAIN_MENU}
    };
    
    stateKeyBindings[GameState::GAME_OVER] = {
        {sf::Keyboard::Num1, CommandType::MAIN_MENU},
        {sf::Keyboard::Num2, CommandType::EXIT}
    };
    
    stateKeyBindings[GameState::UPGRADING] = {
        {sf::Keyboard::Num1, CommandType::UPGRADE_1},
        {sf::Keyboard::Num2, CommandType::UPGRADE_2},
        {sf::Keyboard::Num3, CommandType::UPGRADE_3},
        {sf::Keyboard::Num4, CommandType::UPGRADE_4}
    };
    
    stateKeyBindings[GameState::BUYING_SPELL] = {
        {sf::Keyboard::Num1, CommandType::BUY_SPELL_1},
        {sf::Keyboard::Num2, CommandType::BUY_SPELL_2},
        {sf::Keyboard::Num3, CommandType::BUY_SPELL_3},
        {sf::Keyboard::Num4, CommandType::BUY_SPELL_4},
        {sf::Keyboard::Num5, CommandType::BUY_SPELL_5},
        {sf::Keyboard::Escape, CommandType::CANCEL}
    };
}

std::unique_ptr<Command> InputHandler::handleEvent(const sf::Event& event, GameState::State currentState){
    auto stateBindings = stateKeyBindings.find(currentState);
    if (stateBindings == stateKeyBindings.end())
        return std::make_unique<KeyboardCommand>(CommandType::UNKNOWN);
    
    if (event.type == sf::Event::KeyPressed){
        auto it = stateBindings->second.find(event.key.code);
        if (it != stateBindings->second.end())
            return std::make_unique<KeyboardCommand>(it->second);
    }
    else if (event.type == sf::Event::MouseButtonPressed){
        if (event.mouseButton.button == sf::Mouse::Left)
            return std::make_unique<MouseCommand>(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
    }
    
    return std::make_unique<KeyboardCommand>(CommandType::UNKNOWN);
}

void InputHandler::loadKeyBindings(const std::string& configPath){
    std::ifstream file(configPath);
    if (!file.is_open()){
        std::cout << "Config file not found, using default bindings" << std::endl;
        return;
    }
    
    try {
        nlohmann::json config;
        file >> config;
        
        auto tempBindings = parseConfig(config);
        if (validateConfig(tempBindings)){
            stateKeyBindings = std::move(tempBindings);

            std::cout << "Key bindings loaded successfully from " << configPath << std::endl;
        }
        else {
            std::cout << "Config file contains errors, using default bindings" << std::endl;
            setupDefaultBindings();
        }
        
    }
    catch (const std::exception& e){
        std::cout << "Error loading config: " << e.what() << ", using defaults" << std::endl;
        setupDefaultBindings();
    }

    file.close();
}

std::unordered_map<GameState::State, std::unordered_map<sf::Keyboard::Key, CommandType>> InputHandler::parseConfig(const nlohmann::json& config){
    std::unordered_map<GameState::State, std::unordered_map<sf::Keyboard::Key, CommandType>> tempBindings;
    
    static const std::unordered_map<std::string, sf::Keyboard::Key> keyMap = createKeyMap();
    static const std::unordered_map<std::string, GameState::State> stateMap = createStateMap();
    static const std::unordered_map<std::string, CommandType> commandMap = createCommandMap();
    
    for (const auto& [stateStr, stateConfig] : config.items()){
        auto stateIt = stateMap.find(stateStr);
        if (stateIt == stateMap.end()){
            std::cout << "Error: Unknown state " << stateStr << std::endl;
            continue;
        }
        
        GameState::State gameState = stateIt->second;
        std::unordered_map<sf::Keyboard::Key, CommandType> stateBinding;
        
        for (const auto& [commandStr, keyStr] : stateConfig.items()){
            auto cmdIt = commandMap.find(commandStr);
            auto keyIt = keyMap.find(keyStr);
            
            if (cmdIt == commandMap.end()){
                std::cout << "Error: Unknown command " << commandStr << " in state " << stateStr << std::endl;
                continue;
            }
            
            if (keyIt == keyMap.end()){
                std::cout << "Error: Unknown key " << keyStr << " for command " << commandStr << " in state " << stateStr << std::endl;
                continue;
            }
            
            stateBinding[keyIt->second] = cmdIt->second;
        }
        
        tempBindings[gameState] = stateBinding;
    }
    
    return tempBindings;
}

bool InputHandler::validateConfig(const std::unordered_map<GameState::State, std::unordered_map<sf::Keyboard::Key, CommandType>>& bindings){    
    static const std::vector<GameState::State> requiredStates = {
        GameState::MAIN_MENU, GameState::PLAYING, GameState::LEVEL_COMPLETE,
        GameState::GAME_OVER, GameState::UPGRADING, GameState::BUYING_SPELL
    };
    
    for (auto state : requiredStates){
        if (bindings.find(state) == bindings.end()){
            std::cout << "Error: Missing configuration" << std::endl;
            return false;
        }
    }
    
    for (const auto& [state, stateBindings] : bindings){
        if (!validateStateBindings(state, stateBindings)){
            return false;
        }
    }
    
    for (const auto& [state, _] : bindings) {
        if (std::find(requiredStates.begin(), requiredStates.end(), state) == requiredStates.end()){
            std::cout << "Warning: Unknown state" << std::endl;
            return false;
        }
    }
    
    return true;
}

bool InputHandler::validateStateBindings(GameState::State state, const std::unordered_map<sf::Keyboard::Key, CommandType>& stateBindings){
    std::unordered_map<GameState::State, std::vector<CommandType>> requiredCommands = {
        {GameState::MAIN_MENU, {CommandType::NEW_GAME, CommandType::CONTINUE_GAME, CommandType::EXIT}},
        {GameState::PLAYING, {CommandType::MOVE_UP, CommandType::MOVE_DOWN, CommandType::MOVE_LEFT, CommandType::MOVE_RIGHT,
            CommandType::TOGGLE_FIGHT, CommandType::SHOW_ATTACK_RADIUS, CommandType::BUY_SPELL,
            CommandType::CAST_SPELL_1, CommandType::CAST_SPELL_2, CommandType::CAST_SPELL_3,
            CommandType::CAST_SPELL_4, CommandType::CAST_SPELL_5, CommandType::CAST_SPELL_6,
            CommandType::SAVE_GAME, CommandType::CANCEL}},
        {GameState::LEVEL_COMPLETE, {CommandType::CONTINUE_NEXT_LEVEL, CommandType::MAIN_MENU}},
        {GameState::GAME_OVER, {CommandType::MAIN_MENU, CommandType::EXIT}},
        {GameState::UPGRADING, {CommandType::UPGRADE_1, CommandType::UPGRADE_2, CommandType::UPGRADE_3, CommandType::UPGRADE_4}},
        {GameState::BUYING_SPELL, {CommandType::BUY_SPELL_1, CommandType::BUY_SPELL_2, CommandType::BUY_SPELL_3, CommandType::BUY_SPELL_4, CommandType::BUY_SPELL_5, CommandType::CANCEL}}
    };
    
    auto requiredIt = requiredCommands.find(state);
    if (requiredIt != requiredCommands.end()){
        for (auto requiredCommand : requiredIt->second){
            bool found = false;
            for (const auto& [_, command] : stateBindings){
                if (command == requiredCommand){
                    found = true;
                    break;
                }
            }
            if (!found){
                std::cout << "Error: Missing required command" << std::endl;
                return false;
            }
        }
    }
    
    std::unordered_map<sf::Keyboard::Key, int> keyUsageCount;
    std::unordered_map<CommandType, int> commandUsageCount;
    
    for (const auto& [key, command] : stateBindings){
        keyUsageCount[key]++;
        if (keyUsageCount[key] > 1){
            std::cout << "Error: One key is assigned to multiple commands in one state" << std::endl;
            return false;
        }
        
        commandUsageCount[command]++;
        if (commandUsageCount[command] > 1){
            std::cout << "Error: Command is assigned multiple times in one state" << std::endl;
            return false;
        }
    }
    
    for (const auto& [_, command] : stateBindings) {
        if (commandToString(command) == "UNKNOWN") {
            std::cout << "Error: Unknown command type in state" << std::endl;
            return false;
        }
    }
    
    return true;
}

std::unordered_map<std::string, sf::Keyboard::Key> InputHandler::createKeyMap(){
    return {
        {"Up", sf::Keyboard::Up}, {"Down", sf::Keyboard::Down},
        {"Left", sf::Keyboard::Left}, {"Right", sf::Keyboard::Right},
        {"A", sf::Keyboard::A}, {"B", sf::Keyboard::B}, {"C", sf::Keyboard::C},
        {"D", sf::Keyboard::D}, {"E", sf::Keyboard::E}, {"F", sf::Keyboard::F},
        {"G", sf::Keyboard::G}, {"H", sf::Keyboard::H}, {"I", sf::Keyboard::I},
        {"J", sf::Keyboard::J}, {"K", sf::Keyboard::K}, {"L", sf::Keyboard::L},
        {"M", sf::Keyboard::M}, {"N", sf::Keyboard::N}, {"O", sf::Keyboard::O},
        {"P", sf::Keyboard::P}, {"Q", sf::Keyboard::Q}, {"R", sf::Keyboard::R},
        {"S", sf::Keyboard::S}, {"T", sf::Keyboard::T}, {"U", sf::Keyboard::U},
        {"V", sf::Keyboard::V}, {"W", sf::Keyboard::W}, {"X", sf::Keyboard::X},
        {"Y", sf::Keyboard::Y}, {"Z", sf::Keyboard::Z},
        {"Num0", sf::Keyboard::Num0}, {"Num1", sf::Keyboard::Num1},
        {"Num2", sf::Keyboard::Num2}, {"Num3", sf::Keyboard::Num3},
        {"Num4", sf::Keyboard::Num4}, {"Num5", sf::Keyboard::Num5},
        {"Num6", sf::Keyboard::Num6}, {"Num7", sf::Keyboard::Num7},
        {"Num8", sf::Keyboard::Num8}, {"Num9", sf::Keyboard::Num9},
        {"Escape", sf::Keyboard::Escape}, {"Space", sf::Keyboard::Space},
        {"Enter", sf::Keyboard::Enter}, {"Backspace", sf::Keyboard::BackSpace}
    };
}

std::unordered_map<std::string, GameState::State> InputHandler::createStateMap(){
    return {
        {"MAIN_MENU", GameState::MAIN_MENU},
        {"PLAYING", GameState::PLAYING},
        {"LEVEL_COMPLETE", GameState::LEVEL_COMPLETE},
        {"GAME_OVER", GameState::GAME_OVER},
        {"UPGRADING", GameState::UPGRADING},
        {"BUYING_SPELL", GameState::BUYING_SPELL}
    };
}

std::unordered_map<std::string, CommandType> InputHandler::createCommandMap(){
    return {
        {"MOVE_UP", CommandType::MOVE_UP},
        {"MOVE_DOWN", CommandType::MOVE_DOWN},
        {"MOVE_LEFT", CommandType::MOVE_LEFT},
        {"MOVE_RIGHT", CommandType::MOVE_RIGHT},
        {"TOGGLE_FIGHT", CommandType::TOGGLE_FIGHT},
        {"SHOW_ATTACK_RADIUS", CommandType::SHOW_ATTACK_RADIUS},
        {"BUY_SPELL", CommandType::BUY_SPELL},
        {"SAVE_GAME", CommandType::SAVE_GAME},
        {"CAST_SPELL_1", CommandType::CAST_SPELL_1},
        {"CAST_SPELL_2", CommandType::CAST_SPELL_2},
        {"CAST_SPELL_3", CommandType::CAST_SPELL_3},
        {"CAST_SPELL_4", CommandType::CAST_SPELL_4},
        {"CAST_SPELL_5", CommandType::CAST_SPELL_5},
        {"CAST_SPELL_6", CommandType::CAST_SPELL_6},
        {"CANCEL", CommandType::CANCEL},
        {"NEW_GAME", CommandType::NEW_GAME},
        {"CONTINUE_GAME", CommandType::CONTINUE_GAME},
        {"EXIT", CommandType::EXIT},
        {"CONTINUE_NEXT_LEVEL", CommandType::CONTINUE_NEXT_LEVEL},
        {"MAIN_MENU", CommandType::MAIN_MENU},
        {"UPGRADE_1", CommandType::UPGRADE_1},
        {"UPGRADE_2", CommandType::UPGRADE_2},
        {"UPGRADE_3", CommandType::UPGRADE_3},
        {"UPGRADE_4", CommandType::UPGRADE_4},
        {"BUY_SPELL_1", CommandType::BUY_SPELL_1},
        {"BUY_SPELL_2", CommandType::BUY_SPELL_2},
        {"BUY_SPELL_3", CommandType::BUY_SPELL_3},
        {"BUY_SPELL_4", CommandType::BUY_SPELL_4},
        {"BUY_SPELL_5", CommandType::BUY_SPELL_5}
    };
}

KeyBindingsInfo InputHandler::getKeyBindingsInfo() const{
    KeyBindingsInfo info;
    
    auto fillBindings = [this](GameState::State state, std::unordered_map<std::string, std::string>& target){
        auto stateIt = stateKeyBindings.find(state);
        if (stateIt != stateKeyBindings.end()){
            for (const auto& [key, command] : stateIt->second){
                std::string commandStr = commandToString(command);
                std::string keyStr = keyToString(key);
                target[commandStr] = keyStr;
            }
        }
    };
    
    fillBindings(GameState::MAIN_MENU, info.mainMenuBindings);
    fillBindings(GameState::PLAYING, info.playingBindings);
    fillBindings(GameState::LEVEL_COMPLETE, info.levelCompleteBindings);
    fillBindings(GameState::GAME_OVER, info.gameOverBindings);
    fillBindings(GameState::UPGRADING, info.upgradingBindings);
    fillBindings(GameState::BUYING_SPELL, info.buyingSpellBindings);
    
    return info;
}

std::string InputHandler::commandToString(CommandType command) const{
    static const std::unordered_map<CommandType, std::string> commandToStringMap = {
        {CommandType::MOVE_UP, "MOVE_UP"},
        {CommandType::MOVE_DOWN, "MOVE_DOWN"},
        {CommandType::MOVE_LEFT, "MOVE_LEFT"},
        {CommandType::MOVE_RIGHT, "MOVE_RIGHT"},
        {CommandType::TOGGLE_FIGHT, "TOGGLE_FIGHT"},
        {CommandType::SHOW_ATTACK_RADIUS, "SHOW_ATTACK_RADIUS"},
        {CommandType::BUY_SPELL, "BUY_SPELL"},
        {CommandType::SAVE_GAME, "SAVE_GAME"},
        {CommandType::CAST_SPELL_1, "CAST_SPELL_1"},
        {CommandType::CAST_SPELL_2, "CAST_SPELL_2"},
        {CommandType::CAST_SPELL_3, "CAST_SPELL_3"},
        {CommandType::CAST_SPELL_4, "CAST_SPELL_4"},
        {CommandType::CAST_SPELL_5, "CAST_SPELL_5"},
        {CommandType::CAST_SPELL_6, "CAST_SPELL_6"},
        {CommandType::CANCEL, "CANCEL"},
        {CommandType::NEW_GAME, "NEW_GAME"},
        {CommandType::CONTINUE_GAME, "CONTINUE_GAME"},
        {CommandType::EXIT, "EXIT"},
        {CommandType::CONTINUE_NEXT_LEVEL, "CONTINUE_NEXT_LEVEL"},
        {CommandType::MAIN_MENU, "MAIN_MENU"},
        {CommandType::UPGRADE_1, "UPGRADE_1"},
        {CommandType::UPGRADE_2, "UPGRADE_2"},
        {CommandType::UPGRADE_3, "UPGRADE_3"},
        {CommandType::UPGRADE_4, "UPGRADE_4"},
        {CommandType::BUY_SPELL_1, "BUY_SPELL_1"},
        {CommandType::BUY_SPELL_2, "BUY_SPELL_2"},
        {CommandType::BUY_SPELL_3, "BUY_SPELL_3"},
        {CommandType::BUY_SPELL_4, "BUY_SPELL_4"},
        {CommandType::BUY_SPELL_5, "BUY_SPELL_5"}
    };
    
    auto it = commandToStringMap.find(command);
    return it != commandToStringMap.end() ? it->second : "UNKNOWN";
}

std::string InputHandler::keyToString(sf::Keyboard::Key key) const{
    static const std::unordered_map<sf::Keyboard::Key, std::string> keyToStringMap = {
        {sf::Keyboard::Up, "Up"}, {sf::Keyboard::Down, "Down"},
        {sf::Keyboard::Left, "Left"}, {sf::Keyboard::Right, "Right"},
        {sf::Keyboard::A, "A"}, {sf::Keyboard::B, "B"}, {sf::Keyboard::C, "C"},
        {sf::Keyboard::D, "D"}, {sf::Keyboard::E, "E"}, {sf::Keyboard::F, "F"},
        {sf::Keyboard::G, "G"}, {sf::Keyboard::H, "H"}, {sf::Keyboard::I, "I"},
        {sf::Keyboard::J, "J"}, {sf::Keyboard::K, "K"}, {sf::Keyboard::L, "L"},
        {sf::Keyboard::M, "M"}, {sf::Keyboard::N, "N"}, {sf::Keyboard::O, "O"},
        {sf::Keyboard::P, "P"}, {sf::Keyboard::Q, "Q"}, {sf::Keyboard::R, "R"},
        {sf::Keyboard::S, "S"}, {sf::Keyboard::T, "T"}, {sf::Keyboard::U, "U"},
        {sf::Keyboard::V, "V"}, {sf::Keyboard::W, "W"}, {sf::Keyboard::X, "X"},
        {sf::Keyboard::Y, "Y"}, {sf::Keyboard::Z, "Z"},
        {sf::Keyboard::Num0, "0"}, {sf::Keyboard::Num1, "1"},
        {sf::Keyboard::Num2, "2"}, {sf::Keyboard::Num3, "3"},
        {sf::Keyboard::Num4, "4"}, {sf::Keyboard::Num5, "5"},
        {sf::Keyboard::Num6, "6"}, {sf::Keyboard::Num7, "7"},
        {sf::Keyboard::Num8, "8"}, {sf::Keyboard::Num9, "9"},
        {sf::Keyboard::Escape, "Esc"}, {sf::Keyboard::Space, "Space"},
        {sf::Keyboard::Enter, "Enter"}, {sf::Keyboard::BackSpace, "Backspace"}
    };
    
    auto it = keyToStringMap.find(key);
    return it != keyToStringMap.end() ? it->second : "Unknown";
}