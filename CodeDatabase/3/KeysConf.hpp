#pragma once
#include <SFML/Graphics.hpp>
#include <string>


struct KeysConf {
    sf::Keyboard::Scancode UpKey;
    sf::Keyboard::Scancode DownKey;
    sf::Keyboard::Scancode LeftKey;
    sf::Keyboard::Scancode RightKey;

    sf::Keyboard::Scancode ChangeKey;

    sf::Keyboard::Scancode OneKey;
    sf::Keyboard::Scancode TwoKey;
    sf::Keyboard::Scancode ThreeKey;
    sf::Keyboard::Scancode FourKey;
    sf::Keyboard::Scancode FiveKey;

    sf::Keyboard::Scancode CancelKey;
    sf::Keyboard::Scancode ConfirmKey;

    sf::Keyboard::Scancode LoadKey;
    sf::Keyboard::Scancode NewKey;
    sf::Keyboard::Scancode FinishKey;
    sf::Keyboard::Scancode SaveKey;

    sf::Keyboard::Scancode CloseKey;
};
