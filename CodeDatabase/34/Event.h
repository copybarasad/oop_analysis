#ifndef GAME_EVENT_H
#define GAME_EVENT_H

#include "../../utils/Point.h"
#include <string>
#include <variant>

namespace Game {

enum class EventType {
    GameStarted,
    LevelStarted,
    TurnStarted,
    PlayerMoved,
    PlayerAttacked,
    PlayerCastedSpell,
    PlayerTookDamage,
    PlayerGainedSpell,
    EnemyMoved,
    EnemyAttacked,
    EnemySpawned,
    EnemyDefeated,
    AllySpawned,
    AllyAttacked,
    AllyDefeated,
    TowerAttacked,
    TrapPlaced,
    TrapTriggered,
    LevelWon,
    LevelLost,
    GameSaved,
    GenericMessage
};

struct PositionData {
    Utils::Point from;
    Utils::Point to;
};

struct DamageData {
    std::string dealer;
    std::string target;
    int damage;
    Utils::Point location;
};

struct SpellCastData {
    std::string spellName;
    Utils::Point target;
};

using EventData = std::variant<
    std::monostate,
    PositionData,
    DamageData,
    SpellCastData,
    std::string,
    int,
    Utils::Point 
>;

struct Event {
    EventType type;
    EventData data;
};

}

#endif