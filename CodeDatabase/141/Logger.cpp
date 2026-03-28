#include "Logger.h"
#include <sstream>

void Logger::handle(const GameEvent &event) {
    std::visit([this](auto &&ev) {
        std::ostringstream ss;
        using T = std::decay_t<decltype(ev)>;
        if constexpr (std::is_same_v<T, MoveEvent>) {
            ss << "[MOVE] " << ev.actor << " " << ev.fromX << "," << ev.fromY
                    << " -> " << ev.toX << "," << ev.toY;
        } else if constexpr (std::is_same_v<T, DamageEvent>) {
            ss << "[DAMAGE] " << ev.source << " -> " << ev.target
                    << " : " << ev.amount;
        } else if constexpr (std::is_same_v<T, SpellEvent>) {
            ss << "[SPELL] " << ev.description;
        } else if constexpr (std::is_same_v<T, SpawnEvent>) {
            ss << "[SPAWN] " << ev.who << " at " << ev.x << "," << ev.y;
        }
        sink->write(ss.str());
    }, event);
}
