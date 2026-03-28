#ifndef GAME_INPUTHANDLER_H
#define GAME_INPUTHANDLER_H

#include <SFML/Window/Event.hpp>
#include <map>
#include "../basics/Command.h"

class InputHandler {
public:
    InputHandler();
    CommandType parseEvent(const sf::Event& event) ;
private:
    std::map<sf::Keyboard::Key, CommandType> bindings_;

    void loadBindings();
    void setDefaultBindings();
    static bool loadFromFile(std::map<sf::Keyboard::Key, CommandType>& out);

    static bool stringToKey(const std::string& name, sf::Keyboard::Key& key);
    static bool stringToCommand(const std::string& name, CommandType& cmd);
};

#endif
