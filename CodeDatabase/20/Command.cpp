#include "../include/Command.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>


CommandType Command::get_input(){
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
            return CommandType::MoveUp;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
            return CommandType::MoveDown;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
            return CommandType::MoveLeft;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
            return CommandType::MoveRight;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)){
            return CommandType::ChangeWeapon;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
            return CommandType::CastSpellRight;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
            return CommandType::CastSpellLeft;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
            return CommandType::CastSpellUp;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
            return CommandType::CastSpellDown;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F5)) {
            return CommandType::SaveGame;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F6)) {
            return CommandType::LoadGame;
        }
        else return CommandType::None;

}