#ifndef KEY_BINDINGS_H
#define KEY_BINDINGS_H

#include "Command.h"
#include <SFML/Window/Keyboard.hpp>
#include <map>
#include <string>

class KeyBindings {
public:
    KeyBindings();

    void LoadFromFile(const std::string& filename);
    void SaveToFile(const std::string& filename) const;
    void SetDefaults();

    PlayerAction GetAction(sf::Keyboard::Key key) const;
    sf::Keyboard::Key GetKey(PlayerAction action) const;
    
    bool SetBinding(sf::Keyboard::Key key, PlayerAction action);
    bool IsValid() const;

private:
    sf::Keyboard::Key StringToKey(const std::string& str) const;
    std::string KeyToString(sf::Keyboard::Key key) const;
    PlayerAction StringToAction(const std::string& str) const;
    std::string ActionToString(PlayerAction action) const;

    bool ValidateBindings() const;

    std::map<sf::Keyboard::Key, PlayerAction> key_to_action_;
    std::map<PlayerAction, sf::Keyboard::Key> action_to_key_;
};

#endif // KEY_BINDINGS_H
