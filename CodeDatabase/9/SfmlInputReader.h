#ifndef SFML_INPUT_READER_H
#define SFML_INPUT_READER_H

#include <SFML/Graphics.hpp>
#include "Command.h"
#include "KeyBinding.h"

class SfmlInputReader {
public:
    SfmlInputReader();
    bool PollCommand(sf::RenderWindow& window, Command& outCommand);

private:
    KeyBinding bindings_;
};

#endif