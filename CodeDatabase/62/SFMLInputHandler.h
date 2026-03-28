#pragma once
#include "InputHandler.h"
#include "InputConfig.h"
#include <SFML/Graphics.hpp>
#include <iostream>

// Реализация InputHandler для SFML
class SFMLInputHandler : public InputHandler {
private:
    sf::RenderWindow& window_;
    InputConfig config_;

public:
    SFMLInputHandler(sf::RenderWindow& window, const std::string& configFile = "controls.cfg");
    
    bool processInput(std::vector<Event>& events) override;
};

