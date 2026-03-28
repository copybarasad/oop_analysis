#include "quitcommand.hpp"
#include "event_dispatcher.hpp"
#include "events.hpp"
#include <iostream>

bool QuitCommand::Execute(Game& game) {
    (void)game;
    EventDispatcher::instance().notify(UiMessageEvent("Exiting game..."));
    return false; 
}
