#include "MovementStrategy.h"
#include "../PlayerController.h"
#include "../../models/events/EventNotifier.h"
#include "../../models/events/PrintMessageEvent.h"
#include <iostream>
#include <sstream>

MovementStrategy::MovementStrategy(PlayerController* playerController, char input)
    : playerController(playerController), input(input) {}

bool MovementStrategy::handle() {
    bool moved = false;
    switch (input) {
        case 'w': case 'W': moved = playerController->movePlayer(0, -1); break;
        case 's': case 'S': moved = playerController->movePlayer(0, 1); break;
        case 'a': case 'A': moved = playerController->movePlayer(-1, 0); break;
        case 'd': case 'D': moved = playerController->movePlayer(1, 0); break;
    }
    
    if (!moved) {
        const char* direction = "";
        switch (input) {
            case 'w': case 'W': direction = "вверх"; break;
            case 's': case 'S': direction = "вниз"; break;
            case 'a': case 'A': direction = "влево"; break;
            case 'd': case 'D': direction = "вправо"; break;
        }
        std::ostringstream msg;
        msg << "Нельзя двигаться " << direction << "!";
        PrintMessageEvent moveEvent(msg.str());
        EventNotifier::getInstance().notify(moveEvent);
    }
    return true;
}

