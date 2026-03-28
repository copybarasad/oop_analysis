#ifndef KEY_BINDING_H
#define KEY_BINDING_H

#include <SFML/Window/Keyboard.hpp>
#include <map>
#include <string>
#include "InputAction.h"

class KeyBinding {
public:
    KeyBinding();

    void LoadFromFile(const std::string& filename);

    bool TryGetAction(sf::Keyboard::Key key, InputAction& action) const;

private:
    std::map<sf::Keyboard::Key, InputAction> key_to_action_;

    void SetDefault();
    bool LoadFromJson(const std::string& filename);

    bool ParseKey(const std::string& name, sf::Keyboard::Key& key);
    bool ParseAction(const std::string& name, InputAction& action);

    bool ParseKeyName(const std::string& name, sf::Keyboard::Key& outKey);
    bool ParseActionName(const std::string& name, InputAction& outAction);
};

#endif