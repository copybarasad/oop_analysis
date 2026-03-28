#include "GameLogger.h"
#include <sstream>

GameLogger::GameLogger(GameEventBus& bus, std::unique_ptr<LogSink> sink)
    : sink_(std::move(sink)), bus_(bus) {
    bus_.Subscribe(this);
}

GameLogger::~GameLogger() {
    bus_.Unsubscribe(this);
}

void GameLogger::OnEvent(const GameEvent& event) {
    if (!sink_) return;
    sink_->Write(Format(event));
}

std::string GameLogger::Format(const GameEvent& event) const {
    std::ostringstream out;
    switch (event.type) {
        case GameEventType::PlayerMoved:
            out << event.source << " moved to (" << event.position.X()
                << "," << event.position.Y() << ")";
            break;
        case GameEventType::DamageDealt:
            out << event.source << " dealt " << event.amount
                << " dmg to " << event.target
                << " at (" << event.position.X() << "," << event.position.Y() << ")";
            break;
        case GameEventType::SpellAcquired:
            out << event.source << " acquired spell: " << event.details;
            break;
        case GameEventType::SpellCast:
            out << event.source << " cast spell: " << event.details;
            break;
        case GameEventType::Spawned:
            out << event.source << " spawned at (" << event.position.X()
                << "," << event.position.Y() << ")";
            break;
        case GameEventType::ModeChanged:
            out << event.source << " switched mode to " << event.details;
            break;
        case GameEventType::Saved:
            out << "Game saved to " << event.details;
            break;
        case GameEventType::Loaded:
            out << "Game loaded from " << event.details;
            break;
        case GameEventType::TrapTriggered:
            out << event.source << " triggered trap at (" << event.position.X()
                << "," << event.position.Y() << ") for " << event.amount;
            break;
        default:
            out << "Unknown event";
            break;
    }
    return out.str();
}
