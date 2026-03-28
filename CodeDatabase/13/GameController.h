#ifndef GAME_GAMECONTROLLER_H
#define GAME_GAMECONTROLLER_H
#include <SFML/Window/Event.hpp>
#include "Command.h"

template<typename Controller>
class GameController {
public:
    CommandType parseEvent(const sf::Event &event) { return controller_.parseEvent(event); };

private:
    Controller controller_;
};
#endif
