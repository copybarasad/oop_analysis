#ifndef INPUT_HANDLER
#define INPUT_HANDLER

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>
#include <unordered_map>
#include <json.hpp>
#include <memory>
#include <fstream>
#include <iostream>
#include <vector>

#include "command.hpp"
#include "gameState.hpp"
#include "keyBindingsInfo.hpp"

class InputHandler{
    private:
        std::unordered_map<GameState::State, std::unordered_map<sf::Keyboard::Key, CommandType>> stateKeyBindings;
        
        void setupDefaultBindings();
        
        std::unordered_map<GameState::State, std::unordered_map<sf::Keyboard::Key, CommandType>> parseConfig(const nlohmann::json& config);
        bool validateConfig(const std::unordered_map<GameState::State, std::unordered_map<sf::Keyboard::Key, CommandType>>& bindings);
        bool validateStateBindings(GameState::State state, const std::unordered_map<sf::Keyboard::Key, CommandType>& stateBindings);
        
        std::unordered_map<std::string, sf::Keyboard::Key> createKeyMap();
        std::unordered_map<std::string, GameState::State> createStateMap();
        std::unordered_map<std::string, CommandType> createCommandMap();

        std::string commandToString(CommandType command) const;
        std::string keyToString(sf::Keyboard::Key key) const;
        
    public:
        InputHandler();
        std::unique_ptr<Command> handleEvent(const sf::Event& event, GameState::State currentState);
        void loadKeyBindings(const std::string& configPath);

        KeyBindingsInfo getKeyBindingsInfo() const;
};

#endif