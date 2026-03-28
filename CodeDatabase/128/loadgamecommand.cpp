#include "loadgamecommand.hpp"
#include "gamemodel.hpp"
#include "gameserializer.hpp"
#include "event_dispatcher.hpp"
#include "events.hpp"
#include <iostream>

LoadGameCommand::LoadGameCommand(std::string filename)
    : filename_(std::move(filename)) {}

bool LoadGameCommand::Execute(Game& game) {
    try {
        GameSerializer serializer;
        serializer.LoadGame(game, filename_);
        EventDispatcher::instance().notify(UiMessageEvent("✔ Game loaded"));
        EventDispatcher::instance().notify(TurnCompletedEvent(0, false));
        return true;
    }
    catch (const std::exception& e) {
        std::cout << "❌ Error loading game: " << e.what() << "\n";
        return false;
    }
}
