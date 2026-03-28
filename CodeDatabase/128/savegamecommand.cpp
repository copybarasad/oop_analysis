#include "savegamecommand.hpp"
#include "gamemodel.hpp"
#include "gameserializer.hpp"
#include "event_dispatcher.hpp"
#include "events.hpp"

#include <iostream>

SaveGameCommand::SaveGameCommand(std::string filename)
    : filename_(std::move(filename)) {}

bool SaveGameCommand::Execute(Game& game) {
    try {
        GameSerializer serializer;
        serializer.SaveGame(game, filename_);
        EventDispatcher::instance().notify(UiMessageEvent("✔ Game saved"));
        return true;
    }
    catch (const std::exception& e) {
        std::cout << "❌ Error saving game: " << e.what() << "\n";
        return false;
    }
}
