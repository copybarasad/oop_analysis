#include "InputController/Handlers/sfml/SFMLHandler.hpp"
#include "Serialization/JsonManager.hpp"
#include <iostream>

SFMLInput::SFMLInput(sf::RenderWindow& window) : window(window) {
    JsonManager jm;
    try {
        jm.unmarshal(keys, "static/userkeys.json");
    } catch(FileException& e) {
        std::cerr << e.what() << "\n";
        jm.unmarshal(keys, "static/defaultkeys.json");
    }
}

Command SFMLInput::getCommand() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (event.type == sf::Event::KeyPressed) {
            sf::Keyboard::Scancode sc = event.key.scancode;

            if (sc == keys.UpKey) return Command::Up;
            if (sc == keys.LeftKey) return Command::Left;
            if (sc == keys.DownKey) return Command::Down;
            if (sc == keys.RightKey) return Command::Right;
            if (sc == keys.ChangeKey) return Command::Change;

            if (sc == keys.OneKey) return Command::One;
            if (sc == keys.TwoKey) return Command::Two;
            if (sc == keys.ThreeKey) return Command::Three;
            if (sc == keys.FourKey) return Command::Four;
            if (sc == keys.FiveKey) return Command::Five;

            if (sc == keys.CancelKey) return Command::Cancel;
            if (sc == keys.ConfirmKey) return Command::Confirm;

            if (sc == keys.LoadKey) return Command::Load;
            if (sc == keys.NewKey) return Command::New;
            if (sc == keys.FinishKey) return Command::Finish;
            if (sc == keys.SaveKey) return Command::Save;

            if (sc == keys.CloseKey) window.close();
        }   
    }
    return Command::None;
}


bool SFMLInput::isAlive() {
    return window.isOpen();
}
