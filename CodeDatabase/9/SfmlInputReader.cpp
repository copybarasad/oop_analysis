#include "SfmlInputReader.h"
#include "InputAction.h"

#include <SFML/Graphics.hpp>

class Command;

SfmlInputReader::SfmlInputReader(){
    bindings_.LoadFromFile("controls.json");
}

bool SfmlInputReader::PollCommand(sf::RenderWindow& window, Command& outCommand){
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
            outCommand.type = CommandType::None;
            return true;
        }
        if (event.type == sf::Event::KeyPressed &&
            event.key.code == sf::Keyboard::Escape) {
            window.close();
            return false;
        }
        if (event.type == sf::Event::KeyPressed) {
            InputAction action;
            if (bindings_.TryGetAction(event.key.code, action)) {
                switch (action) {
                case InputAction::MoveUp:
                    outCommand = Command::Move(0, -1);
                    return true;

                case InputAction::MoveDown:
                    outCommand = Command::Move(0, 1);
                    return true;

                case InputAction::MoveLeft:
                    outCommand = Command::Move(-1, 0);
                    return true;

                case InputAction::MoveRight:
                    outCommand = Command::Move(1, 0);
                    return true;

                case InputAction::ToggleAttackMode:
                    outCommand = Command::ToggleAttackMode();
                    return true;

                case InputAction::BuySpell:
                    outCommand = Command::BuySpell();
                    return true;

                case InputAction::SelectCard1:
                    outCommand = Command::SelectCard(0);
                    return true;

                case InputAction::SelectCard2:
                    outCommand = Command::SelectCard(1);
                    return true;

                case InputAction::SelectCard3:
                    outCommand = Command::SelectCard(2);
                    return true;

                case InputAction::SelectCard4:
                    outCommand = Command::SelectCard(3);
                    return true;

                case InputAction::SelectCard5:
                    outCommand = Command::SelectCard(4);
                    return true;

                case InputAction::SelectCard6:
                    outCommand = Command::SelectCard(5);
                    return true;

                case InputAction::SelectCard7:
                    outCommand = Command::SelectCard(6);
                    return true;

                case InputAction::SelectCard8:
                    outCommand = Command::SelectCard(7);
                    return true;

                case InputAction::SelectCard9:
                    outCommand = Command::SelectCard(8);
                    return true;
                case InputAction::SaveGame:
                    outCommand = Command::SaveGame();
                    return true;

                case InputAction::LoadGame:
                    outCommand = Command::LoadGame();
                    return true;
                }
            }

        }

        if (event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left) {

            sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
            const float cellW = 64.f;
            const float cellH = 64.f;

            int gx = static_cast<int>(mouse_pos.x / cellW);
            int gy = static_cast<int>(mouse_pos.y / cellH);

            outCommand = Command::CastSpell({gx, gy});
            return true;
        }
    }

    return false;
}