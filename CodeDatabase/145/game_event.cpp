#include "game_event.h"

std::string GameEvent::typeToString(Type t) {
    switch (t) {
        case Type::PLAYER_MOVED: return "PLAYER_MOVED";
        case Type::PLAYER_ATTACKED: return "PLAYER_ATTACKED";
        case Type::PLAYER_TOOK_DAMAGE: return "PLAYER_TOOK_DAMAGE";
        case Type::PLAYER_DIED: return "PLAYER_DIED";
        case Type::ENEMY_SPAWNED: return "ENEMY_SPAWNED";
        case Type::ENEMY_MOVED: return "ENEMY_MOVED";
        case Type::ENEMY_ATTACKED: return "ENEMY_ATTACKED";
        case Type::ENEMY_TOOK_DAMAGE: return "ENEMY_TOOK_DAMAGE";
        case Type::ENEMY_DIED: return "ENEMY_DIED";
        case Type::ALLY_SPAWNED: return "ALLY_SPAWNED";
        case Type::ALLY_MOVED: return "ALLY_MOVED";
        case Type::ALLY_ATTACKED: return "ALLY_ATTACKED";
        case Type::ALLY_TOOK_DAMAGE: return "ALLY_TOOK_DAMAGE";
        case Type::ALLY_DIED: return "ALLY_DIED";
        case Type::SPELL_CAST: return "SPELL_CAST";
        case Type::SPELL_ACQUIRED: return "SPELL_ACQUIRED";
        case Type::GAME_STARTED: return "GAME_STARTED";
        case Type::LEVEL_COMPLETED: return "LEVEL_COMPLETED";
        case Type::GAME_OVER: return "GAME_OVER";
        default: return "UNKNOWN";
    }
}