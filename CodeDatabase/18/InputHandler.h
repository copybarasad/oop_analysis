#pragma once
#include <SFML/Window.hpp>
#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <memory>
#include <set>
#include "Command.h"

enum class ActionType {
    MoveUp, MoveDown, MoveLeft, MoveRight,
    Spell1, Spell2, Spell3,
    Shop, SwitchMode, Save, Load, Exit, NewGame, Continue,
    Upgrade1, Upgrade2, Upgrade3,
    Unknown
};

class SfmlInputHandler {
private:
    std::map<sf::Keyboard::Key, ActionType> keyMap;
    sf::Window* windowRef = nullptr;
    int selectedSpellIndex = -1;

    sf::Keyboard::Key stringToKey(const std::string& s);
    ActionType stringToAction(const std::string& s);

public:
    SfmlInputHandler();
    void setWindow(sf::Window& win);
    void loadConfig(const std::string& filename);
    void setDefaultConfig();

    std::unique_ptr<Command> handleInput(const sf::Event& event);
    std::unique_ptr<Command> handleMouse(const sf::Event& event, float tileSize);

    int getSelectedSpellIndex() const { return selectedSpellIndex; }
};