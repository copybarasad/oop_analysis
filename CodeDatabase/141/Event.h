#pragma once

#include <string>
#include <variant>

struct MoveEvent {
    std::string actor;
    int fromX;
    int fromY;
    int toX;
    int toY;
};

struct DamageEvent {
    std::string source;
    std::string target;
    int amount;
};

struct SpellEvent {
    std::string description;
};

struct SpawnEvent {
    std::string who;
    int x;
    int y;
};

using GameEvent = std::variant<MoveEvent, DamageEvent, SpellEvent, SpawnEvent>;
